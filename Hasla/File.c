#include "Main.h"

FILE * openFileAndGetNumberOfElements(char * name, int * numberofentries)
{
	char buf[100];
	FILE * tmp = fopen((const char*)name, "r");

	if (tmp != NULL)
	{
		char * buffer = (char *)malloc(sizeof(char) * (MAX_LENGTH_WORD + 1));
		*numberofentries = atoi(fgets(buf, MAX_LENGTH_WORD, tmp));
		fgetc(tmp);
		free(buffer);
	}
	return tmp;
}


