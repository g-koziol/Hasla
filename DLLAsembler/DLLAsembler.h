// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DLLASEMBLER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DLLASEMBLER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DLLASEMBLER_EXPORTS
#define DLLASEMBLER_API __declspec(dllexport)
#else
#define DLLASEMBLER_API __declspec(dllimport)
#endif

// This class is exported from the DLLAsembler.dll
class DLLASEMBLER_API CDLLAsembler {
public:
	CDLLAsembler(void);
	// TODO: add your methods here.
};

extern DLLASEMBLER_API int nDLLAsembler;

DLLASEMBLER_API int fnDLLAsembler(void);

//extern DWORD compare(DWORD word, DWORD wordtofind, DWORD length);