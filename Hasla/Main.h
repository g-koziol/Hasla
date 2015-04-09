//wykorzystane biblioteki 
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <intrin.h>
#include <string.h>
#include <conio.h>
#include <process.h>
#include <time.h>
#include <Mmsystem.h>
#include <tchar.h>
#include <locale.h>

extern DWORD _stdcall comparesse(DWORD,DWORD); //deklaracja funkcji asemblerowej

#define MAX_LENGTH_WORD 32 //maksymalna dlugosc slowa wieksza dla pewnosci
#define LETTERS_NUMBER ('Z'-'A') //ilosc liter alfabetu
#define STORE_START "data\\A.txt" //lokazlizacja pierwszego pliku z baz¹ danych


//deklaracje zmiennych globalnych 
extern double resultTime = 0; //laczny czas wykonania
extern LARGE_INTEGER freq= {0,0}; // 3 zmienne uzywane przez funkcje mierzace czas
extern LARGE_INTEGER endTimefull = {0,0};
extern LARGE_INTEGER startTimefull = {0,0};
extern int CPUInfo[4] = { -1 }; //zmienne przechowuj¹ce informacje o procesorze
extern int nFeatureInfo = 0;
extern boolean bSSE41Extensions = 0;
extern boolean bSSE42Extensions = 0;
extern boolean bMultithreading = 0;

int nLogicalProcessors = 0; //zmienne przechowuj¹ce informacje uruchomieniowe
int outputScreenOption = 0;
int asemblerOption = 0;
int mode = 0;
int actualNumberOfThreads = 0;
wchar_t * outputFile;

/*
Struktura przechowuj¹ca informacje s³owie i nastêpnym elemencie w liscie z³o¿onej z tej struktury 
*/
typedef struct Entry
{
	wchar_t * word;
	struct Entry * next;
} Entry;

/*
Struktura przechowuj¹ca dane przekazywane miêdzy w¹tkami, a programem glównym 
*/
typedef struct Package
{
	wchar_t * word;
	FILE * fileptr;
	Entry * database;
	Entry * results;
	int counter;
}Package;


/*
Deklaracje funkcji z pliku Entry.c, dotycz¹ce operacji na strukturach Entry
*/
//Tworzenie nowej struktury Entry
struct Entry * newEntry(wchar_t * name, int opt);
//dodawanie wyników do istniej¹cej ju¿ bazy wynikow
Entry * addEntryToList(struct Entry * element, struct Entry * database);
//zliczanie elementow
int getNumberOfEntries(struct Entry *);



/*
Deklaracje funkcji z pliku File.c, dotycz¹ce operacji na plikach
*/
//otworz plik i odczytaj liczbe slow
FILE * openFileAndGetNumberOfElements(char *, int *);
//wczytanie bazy danych z pliku
struct Entry * readEntriesFromFile(FILE * fileptr, int strlen);

/*
Deklaracje funkcji z pliku Program.c, dotycz¹ce operacji na plikach
*/
//znajdz wszystkie slowa
extern struct Entry * findAllEntries(wchar_t *, struct Entry *);
//znajdz wszystkie slowa wersja dla podzielonego jednego pliku
struct Entry * findAllEntriesCounted(wchar_t *, struct Entry *, int);
//funckja sterujaca dla otrzymanych wynikow
void output(struct Entry *);
//wysietlenie wynikow na ekranie(nieuzywana)
void outputDatabaseScreen(struct Entry *);
//wypisanie wynikow do pliku
void outputDatabaseFile(struct Entry *);
//czyszczenie listy z pamieci
void clean(struct Entry *);
//rozpocznij program bez watkow
void startProgram(wchar_t *);
//rozpocznij program z watkami
void startProgramWithThreads(wchar_t * wordtofind);
//ustaw wskazniki z podzia³em dla watkow(nieuzywana)
Entry ** setPointers(Entry *, int **);
//przeszukaj zbiory watkami z licznikiem (nieuzywana)
unsigned __stdcall searchThreadCounted(void * arg);
//przeszukaj zbior watkami bez licznika 
unsigned __stdcall searchThreadUncounted(void * arg);
//utworz paczke dla watku
Package * setPackage(wchar_t * word, Entry * database, Entry * results, FILE * fileptr, int);
//skasuj paczke dla watku
Package * deletePackage(Package * pack);
//odczytaj liczbe rdzeni
int readNumberOfLogicalUnits(void);
//zamien char na wchar
wchar_t * chartowchar(const char * tab);
//zamien wchar na char
char * wchartochar(wchar_t * name);
//funkcja wywolujaca algorytm porownania, w zaleznosci od parametrow wersje asemblerowa lub w jezyku C
wchar_t * compareEntry(wchar_t * word, wchar_t * wordtofind, int length);


