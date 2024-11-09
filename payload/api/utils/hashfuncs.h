#ifndef PAYLOAD_HASHFUNCS_H
#define PAYLOAD_HASHFUNCS_H
#include "../ntshit/structs.h"

FARPROC get_proc_address(HMODULE hModule, const char* ImportName); // todo - encrypt arguments
HMODULE get_module_handle(const char* ModuleName);
VOID Memcpy(IN PVOID pDestination, IN PVOID pSource, SIZE_T sLength);

#endif