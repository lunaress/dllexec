/*
 *
 * dllexec 0.1
 * author: lunaress
 * website: https://lunaress.moe
 *
 */

#include <Windows.h>
#include <stdio.h>

const char* helpText = "dllexec: usage:\n\
                        \rdllexec.exe <dll> <function>\n";


int main(int argc, char* argv[]) {
    // not the best way to "parse" arguments but it's enough
    if ((argc < 3) || (argc > 3)) {
        printf("%s", helpText);
        return 1;
    }

    char* lib = argv[1];
    char* func = argv[2];


    // if the directory exists but not the file, it will continue and return error 126 in the LoadLibrary part below
    // this could be mitigated by using something like PathFileExists but i didn't find it necessary
    if ((GetFileAttributesA(lib) == INVALID_FILE_ATTRIBUTES) && (GetLastError() == ERROR_FILE_NOT_FOUND)) {
        printf("dllexec: invalid file");
        return 1;
    }


    HMODULE loadedLibrary = LoadLibraryA(lib);
    if (loadedLibrary == NULL) {
        printf("dllexec: error: %d\n", GetLastError());
        return 1;
    }


    // execute the given function as well as DllMain
    printf("dllexec: executing\n");
    FARPROC executeFunction = GetProcAddress(loadedLibrary, func);
    if (executeFunction == NULL) {
        printf("dllexec: could not find function\n");
        return 1;
    }


    // print the output (function won't necessarily have any)
    char* res = (char*)executeFunction();

    printf("%s\n", res == NULL ? "dllexec: no response" : res);
    printf("dllexec: finished");

    return 0;
}