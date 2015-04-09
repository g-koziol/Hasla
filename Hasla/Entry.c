#include "Main.h"

Entry * newEntry(wchar_t * name, int opt)
{
	Entry * tmp = NULL;
	if (name != NULL)
	{
		tmp = (Entry *)malloc(sizeof(Entry));
		tmp->word = (wchar_t *)malloc((wcslen(name) + 1) * sizeof(wchar_t));
		wcscpy(tmp->word, name);
		if (opt == 1)
			tmp->word[wcslen(name)] = 0;
		tmp->next = NULL;
	}
	return tmp;
}

Entry * addEntryToList(Entry * element, Entry * database)
{
	Entry * tmpbase = database;
	if (database == NULL)
	{
		database = element;
		return database;
	}
	else
		while (tmpbase->next != NULL)
			tmpbase = tmpbase->next;
	tmpbase->next = element;
	return database;
}

Entry * readEntriesFromFile(FILE * fileptr, int strlen)
{
	Entry * database = NULL, *datatmp = database, *tmppointer = NULL;
	wchar_t * tmp2 = NULL;
	wchar_t * tmp = (wchar_t *)malloc(sizeof(wchar_t) * (MAX_LENGTH_WORD));
	int i = 0;
	if (fileptr != NULL)
	{
		while (!feof(fileptr))
		{
			tmp = fgetws(tmp, MAX_LENGTH_WORD, fileptr);
			tmp[wcslen(tmp) - 1] = L'\0';
			if ((i = wcslen(tmp)) != strlen)
				continue;
			datatmp = newEntry(tmp, 0);
			if (tmppointer != NULL)
				tmppointer->next = datatmp;
			tmppointer = datatmp;
			if (database == NULL)
				database = datatmp;
			if (datatmp != NULL)
				datatmp = datatmp->next;
		}
	}
	else
		//fwprintf(stdout, L"Nie udalo sie wczytac danych z pliku\n");
	free(tmp);
	return database;
}



int getNumberOfEntries(Entry * database)
{
	int i = 0;

	if (database != NULL)
	{
		while (database != NULL)
		{
			i++;
			database = database->next;
		}
	}
	return i;
}












