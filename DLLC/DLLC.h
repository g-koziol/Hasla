#pragma once
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DLLC_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DLLC_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DLLC_EXPORTS
#define DLLC_API __declspec(dllexport)
#else
#define DLLC_API __declspec(dllimport)
#endif


// This class is exported from the DLLC.dll

typedef unsigned char boolean;


extern DLLC_API int nDLLC;

DLLC_API wchar_t * compare(wchar_t * word, wchar_t * wordtofind);
