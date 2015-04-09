#include "Main.h"


Entry * newEntry(char * name, int opt)
{
	Entry * tmp=NULL;
	if(name!=NULL)
	{
		tmp=(Entry *)malloc(sizeof(Entry));
		tmp->word=(char *) malloc((strlen(name)+1) * sizeof(char));
		strncpy(tmp->word,name,strlen(name));
		if(opt==1)
			tmp->word[strlen(name)-1]=0;
		else
			tmp->word[strlen(name)]=0;
			
		tmp->next=NULL;
	}
	return tmp;
}

Entry * addEntryToList(Entry * element,Entry * database)
{
	Entry * tmpbase=database;
	if(database==NULL)
	{
	    database=element;
		return database;
	}
	else
		while(tmpbase->next!=NULL)
			tmpbase=tmpbase->next;
	tmpbase->next=element;
	return database;
}
Entry * readEntriesFromFile(FILE * fileptr)
{
	Entry * database=NULL,*datatmp=database, * tmppointer=NULL;
	char * tmp=NULL;
	if(fileptr!=NULL)
	{
		while(!feof(fileptr))
		{			
			tmp=(char *)malloc(sizeof(char) * MAX_LENGTH_WORD);
			datatmp=newEntry(fgets(tmp,MAX_LENGTH_WORD,fileptr),1);
			if(tmppointer!=NULL)
				tmppointer->next=datatmp;
			tmppointer=datatmp;
			if(database==NULL)
				database=datatmp;
			free(tmp);
			tmp=NULL;
			if (datatmp!=NULL)
				datatmp=datatmp->next;
		}
	}
	else
		printf("Nie udalo sie wczytac danych z pliku\n");
	return database;
}

void writeEntriesToFileOrScreen(FILE * fileptr,Entry * database, boolean opt)
{
	if(database!=NULL)
	{
		while(database!=NULL)
		{
			if(opt==0)
				fputs(database->word,fileptr);
			else
				puts(database->word);
			database=database->next;
		}		
	}
}

int getNumberOfEntries(Entry * database)
{
	int i=0;

	if(database!=NULL)
	{
		while(database!=NULL)
		{
			i++;
			database=database->next;
		}
	}
	return i;
}

Package * setPackage(char * word, Entry * database, Entry * results, FILE * fileptr , int counter)
{
	Package * package=(Package *) malloc (sizeof(Package));

	package->word=(char *) calloc(strlen(word)+1,sizeof(char));
	strcpy(package->word,word);
	package->database=database;
	package->results=results;
	package->counter=counter;
	package->fileptr=fileptr;
	return package;
}

Package * deletePackage(Package * pack)
{
	free(pack->word);
	free(pack);
	return NULL;
}










