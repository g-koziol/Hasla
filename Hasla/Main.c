#include "Main.h"
/*
Paramtery wywolania
1 : szukane slowo
2 : plik wyjsciowy inny niz domyslny
3 : 1 - asembler, 0 - wysoki poziom
4 : ile rdzeni
*/
int wmain(int argc, wchar_t * argv[])
{
	//pobranie informacji o procesorze
	QueryPerformanceFrequency(&freq);
	__cpuid(CPUInfo, 1);
	bSSE41Extensions = (CPUInfo[2] & 0x80000) || 0;
	bSSE42Extensions = (CPUInfo[2] & 0x100000) || 0;
	nFeatureInfo = CPUInfo[3];
	bMultithreading = (nFeatureInfo & (1 << 28)) || 0;	
	if(argc < 4) //jesli za malo parametrow
	{
		printf("Bledna liczba parametrów programu!\n");
		fflush(stdin);
		getchar();
		return 0;
	}
	else 
	{
		if(wcscmp(argv[2],L"null")) //jesli nie podano innego pliku wyjsciowego
		{
			outputFile = (wchar_t *) malloc (wcslen(argv[3]) * sizeof(wchar_t));
			wcscpy(outputFile,argv[2]);
		} else 
			outputFile = NULL;

		if(_wtoi(argv[3]) == 1 ) //jesli wlaczono opcje asemblera
		{
			if(bSSE42Extensions && bSSE41Extensions)
				asemblerOption = 1;
			else{
				printf("Niestety ale program sie nie uruchomi w wersji asemblerowej ze wzgledu na procesor nie obslugujacy wymaganych rozkazow");
				getchar();
				return;
			}
		}
		QueryPerformanceCounter(&startTimefull);
		if(argc < 5)							//jesli jest mniej parametrow program bez podzialu na watki
			startProgram(argv[1]);		
		else									//jesli podana liczba -> liczba rdzeni
		{ 
			actualNumberOfThreads = _wtoi(argv[4]);
			startProgramWithThreads(argv[1]);
		}
		QueryPerformanceCounter(&endTimefull);
		resultTime += (endTimefull.QuadPart - startTimefull.QuadPart) * 1000.0 / freq.QuadPart;
		fwprintf(stdout, L"\nExecution time: %d\n",  (long)resultTime);
	}

	return 0;
}