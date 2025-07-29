#pragma once

//DLL LOADER AND FUNCTION CALLER HELPER

#ifdef CDLL_EXPORTS
#define CDLL_API __declspec(dllexport)
#else
#define CDLL_API __declspec(dllimport)
#endif
#include <Windows.h>
#include <iostream>
#include <string>
using namespace std;

typedef int(__cdecl* FunctionWithSingleParam)(void*);
typedef int(__cdecl* FunctionWithTwoParam)(void*, void*);

class DLL {
    HINSTANCE hinstLib;
    BOOL fFreeResult, fRunTimeLinkSuccess = FALSE;

    static void errFnNotFound(void* ptr) {

    }
public:
    void* Fn(string func) {
        void* ff = GetProcAddress(hinstLib, func.c_str());
        printf(ff ? "%s(..);" : "%s(..) Not Found\n", func.c_str());
        return (ff ? ff : DLL::errFnNotFound);
    }

    DLL(string path) {
        hinstLib = LoadLibraryA(path.c_str());
        if (hinstLib == nullptr) {
            FILE* f; fopen_s(&f, "copydll.txt", "w+");
            if (f) fclose(f);
        }
        printf(hinstLib ? "%s:" : "%s Not Found\n", path.c_str());
    }

    ~DLL() {
        fFreeResult = FreeLibrary(hinstLib);
    }

    int mFn(string func, void* param1 = nullptr) {
        ((FunctionWithSingleParam)Fn(func.c_str()))(param1);
        return 0;
    }

};
