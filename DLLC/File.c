#include "Main.h"

FILE * openFileAndGetNumberOfElements(char * name,int * numberofentries)
{
	FILE * tmp=fopen(name,"r");

	if(tmp!=NULL)
	{
		char * chartmp=(char *)malloc(sizeof(char) * MAX_LENGTH_WORD);
		*numberofentries=atoi(fgets(chartmp,MAX_LENGTH_WORD,tmp));
		free(chartmp);
	}
	return tmp;
}


