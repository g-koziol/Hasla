// DLLC.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"


wchar_t * compare(wchar_t * word, wchar_t * wordtofind)
{		
	int i=0; 
	wchar_t z1=0, z2=0; 
	while(wordtofind[i++]!=L'\0')
	{
		z1=wordtofind[i];				//przypisanie liter z stringow do zmiennych
		z2=word[i];
		if(z1==L' ')					//jesli znak spacji w kluczu kontynuuj
			continue;		
		else if (z1==z2)				//jesli takie same kontynuuj
			continue;
		else 
			return NULL;		        //jesli to nie znak spacji, ani pasujca litera zwroc null
	}
	return word;						//jesli nie bylo roznicy zawroc wskaznik na aktualne slowo
}



// This is the constructor of a class that has been exported.
// see DLLC.h for the class definition

