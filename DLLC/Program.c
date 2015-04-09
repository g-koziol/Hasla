#include 

char * GetNameOfFile(char ** tab)
{
	return tab[3];
}

char * getSearchedWord(char ** tab)
{
	return tab[2];
}


Entry * findAllEntries(char * wordtofind,Entry * database)
{
	Entry * results=NULL, * tmpptr=database, * tmpptr2=NULL;

	if(database==NULL)
		return NULL;
	while(database!=NULL)
	{
		if(strstr(database->word, wordtofind)!=NULL)
			results=addEntryToList(newEntry(database->word,0),results);
		free(database->word);
		database=database->next;		
	}
	while(tmpptr!=NULL)
	{
		tmpptr2=tmpptr;		
		tmpptr=tmpptr->next;
		free(tmpptr2);
	}
	return results;
}

Entry * findAllEntriesCounted(char * wordtofind, Entry * database, int counter)
{
	Entry * results=NULL, * tmpptr = NULL;
	int tmpcounter=0;

	if(database==NULL)
		return NULL;
	while(database!=NULL)
	{
		if(strstr(database->word, wordtofind)!=NULL)
			results=addEntryToList(newEntry(database->word,0),results);
		database=database->next;
		if(tmpcounter==counter)
			break;
		else tmpcounter++;
	}
	return results;
}


void output(Entry * database,int opt,FILE * fileptr,char * filename)
{
	if(opt!=0)
	{
		if((fileptr!=NULL) && (filename!=NULL))
			outputDatabaseFile(database,fileptr,filename);
		else if(opt==1)
			outputDatabaseScreen(database);
		else if(opt==2)
		{
			if((fileptr==NULL) || (filename==NULL))
				outputDatabaseFile(database,fileptr,filename);
			outputDatabaseScreen(database);
		}
	}
}

void outputDatabaseScreen(Entry * database)
{	
	if(database!=NULL)
		while(database!=NULL)
		{
			puts(database->word);
			database=database->next;
		}
	else
		puts("Brak wynikow!");
}


void outputDatabaseFile(Entry * database,FILE * fileptr,char * filename)
{
	char * tmp=(char *) malloc(sizeof(char) * MAX_LENGTH_WORD);
	FILE * tmpfile=NULL;
	if(database!=NULL)
	{
		tmpfile=fopen(filename,"w");//zmiana freopen na fopen
		fputs(_itoa(getNumberOfEntries(database),tmp,10),tmpfile);
		fputs("\n",tmpfile);
		while(database!=NULL)
		{
			fputs(database->word,tmpfile);
			fputs("\n",tmpfile);
			database=database->next;
		}
	}
	fclose(tmpfile);
	free(tmp);
}

void clean(Entry * database)
{
	Entry * tmp=NULL;

	if(database==NULL)
		return;
	while(database!=NULL)
	{
		tmp=database;
		database=database->next;
		free(tmp->word);
		tmp->word=NULL;
		free(tmp);
		tmp=NULL;
	}
}

void startProgram(char * wordtofind, char * outputfilename)
{
	FILE * fileptr=NULL,*fileptr2=NULL;
	Entry * database=NULL,*result=NULL;
	int listlength=0;
	char filename[]=STORE_START;
	printf("Tryb normalny.. \n");
	startTimefull=timeGetTime();

	for(;filename[5]<=90;filename[5]++)
	{
		fileptr=openFileAndGetNumberOfElements(filename,&listlength);
		printf("%s\n",filename);
		database=readEntriesFromFile(fileptr);
		if(database!=NULL)			
			result=addEntryToList(findAllEntries(wordtofind,database),result);
	}
	endTimefull=timeGetTime();
	resultTime=(endTimefull-startTimefull);
	printf("Execution time for %d units: %d\n", nLogicalProcessors, resultTime);
	getchar();
	if(outputfilename!=NULL)
		output(result,2,fileptr2,outputfilename);
	else 
		output(result,1,NULL,NULL);
	
	printf("\n");
	clean(result);


}
void startProgramWithThreads(char * wordtofind, char * outputfilename)
{
	FILE * fileptr=NULL,*fileptr2=NULL;
	Entry *results=NULL;
	int listlength=0, threadcounter=0, counter=0, lastfor=nLogicalProcessors=readNumberOfLogicalUnits();
	unsigned int threadid=0;
	char filename[]=STORE_START;
	HANDLE ** hThreads=(HANDLE **)calloc(nLogicalProcessors,sizeof(HANDLE *));
	Package ** packages=(Package **)calloc(nLogicalProcessors,sizeof(Package *));
	Entry ** database=(Entry **) calloc(nLogicalProcessors,sizeof(Entry *));
	nLogicalProcessors=(nLogicalProcessors==0 ? 1 : nLogicalProcessors);
	printf("Tryb szybki... \n");
	startTimefull=timeGetTime();
	while(1)
	{
		while(threadcounter < nLogicalProcessors)
		{
			printf("%s\n",filename);
			fileptr=openFileAndGetNumberOfElements(filename,&listlength);
			hThreads[threadcounter]=(HANDLE *)malloc(sizeof(HANDLE));
			packages[threadcounter]=setPackage(wordtofind,NULL,NULL,fileptr,0);
			if(fileptr!=NULL)
				*hThreads[threadcounter]=(HANDLE) _beginthreadex(NULL,0,searchThreadUncounted,(void *)packages[threadcounter],0,&threadid);
			else *hThreads[threadcounter]=0;

			counter++;
			threadcounter++;
			filename[5]++;			
			if(filename[5]>'Z')
			{
				lastfor=threadcounter;
				break;
			}
		}

		for(threadcounter=0;(threadcounter < nLogicalProcessors) && (threadcounter < lastfor)  ; threadcounter++)
		{
			WaitForSingleObject(*hThreads[threadcounter],INFINITE);
		}

		for(threadcounter=0;(threadcounter < nLogicalProcessors) && (threadcounter < lastfor); threadcounter++)
		{
			results=addEntryToList(packages[threadcounter]->results,results);
			if(*hThreads[threadcounter]!=0)
				CloseHandle(*hThreads[threadcounter]);
			free(hThreads[threadcounter]);
			free(database[threadcounter]);
			deletePackage(packages[threadcounter]);
		}
		
		if(counter>=LETTERS_NUMBER)
			break;
		else
			threadcounter=0;
	}
	endTimefull=timeGetTime();
	resultTime=(endTimefull-startTimefull);
	printf("Execution time for %d units: %d\n", nLogicalProcessors, resultTime);
	getchar();
	if(outputfilename!=NULL)
	output(results,2,fileptr2,outputfilename);
	else 
	output(results,1,NULL,NULL);
	fflush(stdin);
	getchar();
	clean(results);
	free(packages);
	free(hThreads);
	free(database);
}

Entry * useSpeedToFindOneFile(char * word, Entry * database)
{	
	int ** tab=(int **)calloc(nLogicalProcessors,sizeof(int *));
	unsigned int threadid=0;
	Package ** packages=(Package **)calloc(nLogicalProcessors,sizeof(Package *)); 
	HANDLE ** hThreads=(HANDLE **)calloc(nLogicalProcessors,sizeof(HANDLE *));
	//HANDLE ** hEvents=(HANDLE **)calloc(nLogicalProcessors,sizeof(HANDLE *));
	Entry * results=NULL, ** entryptr ;
	int i=0, leftentries=getNumberOfEntries(database), diff=leftentries / nLogicalProcessors, counter=0, counter2=0;	

	for(;i<nLogicalProcessors;i++)
	{
		tab[i]=(int *)malloc(sizeof(int));
		*(tab[i])=counter;
		counter=counter+diff;
		leftentries=-diff;;
	}

	entryptr=setPointers(database,tab);
	i=0;
	for(;i<nLogicalProcessors;i++)
	{		
		hThreads[i]=(HANDLE *)malloc(sizeof(HANDLE));
		packages[i]=setPackage(word,entryptr[i],NULL,NULL, diff-1);
		*hThreads[i]=(HANDLE) _beginthreadex(NULL,0,searchThreadCounted,(void *)packages[i],0,&threadid);
	}
	for(i=0;i<nLogicalProcessors;i++)
	{	
		WaitForSingleObject(*hThreads[i],INFINITE);
	}

	for(i=0;i<nLogicalProcessors;i++)
	{
		results=addEntryToList(packages[i]->results,results);
		CloseHandle(*hThreads[i]);
		free(hThreads[i]);
		free(tab[i]);	
		entryptr[i]=NULL;
	}
	free(hThreads);
	free(tab);
	free(entryptr);
	return results;
}

Entry ** setPointers(Entry * database, int ** indicators)
{
	Entry ** tab=(Entry **) calloc(nLogicalProcessors,sizeof(Entry *));
	Entry * tmp=database;
	int i=0, counter=1;

	for(;i<nLogicalProcessors;i++)
	{
		while(counter<*(indicators[i]))
		{
			tmp=tmp->next;
			counter++;
		}
		tab[i]=tmp;
	}

	return tab;
}

unsigned __stdcall searchThreadCounted(void * arg)
{
	Package * pack=(Package *)arg;
	Entry * results=NULL;	

	results=findAllEntriesCounted(pack->word,pack->database,pack->counter);
	pack->results=results;
	_endthreadex(0);

	return 0;
}

unsigned __stdcall searchThreadUncounted(void * arg)
{
	Package * pack=(Package *)arg;
	Entry * results=NULL, * database=NULL;	

	database=readEntriesFromFile(pack->fileptr);
	results=findAllEntries(pack->word,database);
	pack->results=results;
	fclose(pack->fileptr);
	pack->fileptr=NULL;
	_endthreadex(0);

	return 0;
}

int readNumberOfLogicalUnits(void)
{
	__cpuid(CPUInfo,1);
	return ((CPUInfo[1] >> 16) & 0xff)-1;
}