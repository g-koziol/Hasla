#include "Main.h"
#include "DLLC.h"


Entry * findAllEntries(wchar_t * wordtofind, Entry * database)
{
	Entry * results = NULL, *tmpptr = database, *tmpptr2 = NULL;

	if (database == NULL)
		return NULL;
	while (database != NULL)
	{		
		if (compareEntry(database->word, wordtofind, wcslen(wordtofind)) != NULL)
			results = addEntryToList(newEntry(database->word, 0), results);		
		free(database->word);
		database = database->next;
	}
	while (tmpptr != NULL)
	{
		tmpptr2 = tmpptr;
		tmpptr = tmpptr->next;
		free(tmpptr2);
	}
	return results;
}

Entry * findAllEntriesCounted(wchar_t * wordtofind, Entry * database, int counter)
{
	Entry * results = NULL, *tmpptr = NULL;
	int tmpcounter = 0;

	if (database == NULL)
		return NULL;
	while (database != NULL)
	{
		if (compareEntry(database->word, wordtofind, wcslen(wordtofind)) != NULL)
			results = addEntryToList(newEntry(database->word, 0), results);
		database = database->next;
		if (tmpcounter == counter)
			break;
		else tmpcounter++;
	}
	return results;
}

void output(Entry * database)
{	
	outputDatabaseFile(database);		
}

void outputDatabaseScreen(Entry * database)
{
	if (database != NULL)
		while (database != NULL)
		{
			fputws(database->word, stdout);
			fputwc(L'\n', stdout);
			database = database->next;
		}
	else
		puts("Brak wynikow!");
}


void outputDatabaseFile(Entry * database)
{
	char * tmp = (char *)malloc(sizeof(char) * MAX_LENGTH_WORD);
	FILE * tmpfile = NULL;

	if(outputFile!=NULL)
		tmpfile = _wfopen(outputFile, L"w");
	else 
		tmpfile = fopen("wyjscie.txt", "w");

	if (database != NULL)
	{
		fputs(_itoa(getNumberOfEntries(database), tmp, 10), tmpfile);
		fputs("\n", tmpfile);
		while (database != NULL)
		{
			fputws(database->word, tmpfile);
			fputws(L"\n", tmpfile);
			database = database->next;
		}
	}
	fputs("Brak wynikow",tmpfile);
	fclose(tmpfile);
	free(tmp);
}

void clean(Entry * database)
{
	Entry * tmp = NULL;

	if (database == NULL)
		return;
	while (database != NULL)
	{
		tmp = database;
		database = database->next;
		free(tmp->word);
		tmp->word = NULL;
		free(tmp);
		tmp = NULL;
	}
}

void startProgram(wchar_t * wordtofind)
{
	FILE * fileptr = NULL, *fileptr2 = NULL;
	Entry * database = NULL, *result = NULL;
	int listlength = 0;
	char * filename = (char *)malloc(sizeof(char) * 100);
	strcpy(filename, STORE_START);

	fwprintf(stdout, L"Tryb bez watkow.. \n");
	resultTime = 0;

	for (; filename[5] <= 90; filename[5]++)
	{
		fileptr = openFileAndGetNumberOfElements(filename, &listlength);
		fwprintf(stdout, L".");
		database = readEntriesFromFile(fileptr, wcslen(wordtofind));
		if (database != NULL)
			result = addEntryToList(findAllEntries(wordtofind, database), result);
	}	
	output(result);
	clean(result);
}

void startProgramWithThreads(wchar_t * wordtofind)
{
	FILE * fileptr = NULL, *fileptr2 = NULL;
	Entry *results = NULL;
	int listlength = 0, threadcounter = 0, counter = 0, lastfor = nLogicalProcessors = (actualNumberOfThreads == 0 ? 1 : actualNumberOfThreads);
	unsigned int threadid = 0;
	char * filename = (char *)malloc(sizeof(char) * 100);
	HANDLE ** hThreads = (HANDLE **)calloc(nLogicalProcessors, sizeof(HANDLE *));
	Package ** packages = (Package **)calloc(nLogicalProcessors, sizeof(Package *));
	Entry ** database = (Entry **)calloc(nLogicalProcessors, sizeof(Entry *));

	strcpy(filename, STORE_START);
	fwprintf(stdout, L"Tryb wielow¹tkowy... \n");
	resultTime = 0;
	while (1)
	{
		while (threadcounter < nLogicalProcessors)
		{
			fwprintf(stdout, L".");
			fileptr = openFileAndGetNumberOfElements(filename, &listlength);
			hThreads[threadcounter] = (HANDLE *)malloc(sizeof(HANDLE));
			packages[threadcounter] = setPackage(wordtofind, NULL, NULL, fileptr, 0);
			if (fileptr != NULL)
				*hThreads[threadcounter] = (HANDLE)_beginthreadex(NULL, 0, searchThreadUncounted, (void *)packages[threadcounter], 0, &threadid);
			else *hThreads[threadcounter] = 0;

			counter++;
			threadcounter++;
			filename[5]++;
			if (filename[5] > 'Z')
			{
				lastfor = threadcounter;
				break;
			}
		}
		for (threadcounter = 0; (threadcounter < nLogicalProcessors) && (threadcounter < lastfor); threadcounter++)
		{
			WaitForSingleObject(*hThreads[threadcounter], INFINITE);
		}
		for (threadcounter = 0; (threadcounter < nLogicalProcessors) && (threadcounter < lastfor); threadcounter++)
		{
			results = addEntryToList(packages[threadcounter]->results, results);
			if (*hThreads[threadcounter] != 0)
				CloseHandle(*hThreads[threadcounter]);
			free(hThreads[threadcounter]);
			free(database[threadcounter]);
			deletePackage(packages[threadcounter]);
		}

		if (counter >= LETTERS_NUMBER)
			break;
		else
			threadcounter = 0;
	}
	output(results);
	clean(results);
	free(packages);
	free(hThreads);
	free(database);
}

Entry ** setPointers(Entry * database, int ** indicators)
{
	Entry ** tab = (Entry **)calloc(nLogicalProcessors, sizeof(Entry *));
	Entry * tmp = database;
	int i = 0, counter = 1;

	for (; i < nLogicalProcessors; i++)
	{
		while (counter < *(indicators[i]))
		{
			tmp = tmp->next;
			counter++;
		}
		tab[i] = tmp;
	}

	return tab;
}

unsigned __stdcall searchThreadCounted(void * arg)
{
	Package * pack = (Package *)arg;
	Entry * results = NULL;

	results = findAllEntriesCounted(pack->word, pack->database, pack->counter);
	pack->results = results;
	_endthreadex(0);

	return 0;
}

unsigned __stdcall searchThreadUncounted(void * arg)
{
	Package * pack = (Package *)arg;
	Entry * results = NULL, *database = NULL;

	database = readEntriesFromFile(pack->fileptr, wcslen(pack->word));
	results = findAllEntries(pack->word, database);
	pack->results = results;
	fclose(pack->fileptr);
	pack->fileptr = NULL;
	_endthreadex(0);

	return 0;
}

int readNumberOfLogicalUnits(void)
{
	__cpuid(CPUInfo, 1);
	return ((CPUInfo[1] >> 16) & 0xff)-1;
}

wchar_t * chartowchar(const char * tab)
{
	wchar_t * tab2 = (wchar_t *)malloc(sizeof(wchar_t) * 100);
	int i = 0, j = 0;
	while (tab[i] != '\0')
	{
		tab2[i] = tab[i];
		i++;
	}
	tab2[i] = '\0';
	return tab2;
}

char * wchartochar(wchar_t * name)
{
	int i = 0;
	char buf[100];
	while (name[i] != '\0')
	{
		if (name[i] == L'\\')
			buf[i] = '\\';
		else buf[i] = (char)name[i];
		i++;
	}
	buf[i] = '\0';
	return buf;
}

wchar_t * compareEntry(wchar_t * word, wchar_t * wordtofind, int length)
{
	wchar_t * ptr = NULL;
	
	if (asemblerOption == 0)
	{
		ptr = compare(word, wordtofind);
	} 
	else 
	{
		ptr=(wchar_t *)comparesse((DWORD)word,(DWORD)wordtofind);
	}	
	return ptr;
}

Package * setPackage(wchar_t * word, Entry * database, Entry * results, FILE * fileptr, int counter)
{
	Package * package = (Package *)malloc(sizeof(Package));

	package->word = (wchar_t *)calloc(wcslen(word) + 1, sizeof(wchar_t));
	wcscpy(package->word, word);
	package->database = database;
	package->results = results;
	package->counter = counter;
	package->fileptr = fileptr;
	return package;
}

Package * deletePackage(Package * pack)
{
	free(pack->word);
	free(pack);
	return NULL;
}
