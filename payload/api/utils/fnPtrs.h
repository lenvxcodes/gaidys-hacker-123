#ifndef PAYLOAD_FNPTRS_H
#define PAYLOAD_FNPTRS_H

#include <windows.h>

typedef HMODULE (WINAPI* fnLoadLibraryA)(IN LPCSTR lpLibFileName);

#endif //PAYLOAD_FNPTRS_H
