#include "hashfuncs.h"
#include "fnPtrs.h"

VOID Memcpy(IN PVOID pDestination, IN PVOID pSource, SIZE_T sLength)
{

    PBYTE D = (PBYTE)pDestination;
    PBYTE S = (PBYTE)pSource;

    while (sLength--)
        *D++ = *S++;
}

FARPROC get_proc_address(HMODULE hModule, const char* ImportName) {
    PBYTE base = (PBYTE)hModule;
    PIMAGE_NT_HEADERS img_nt_headers = (PIMAGE_NT_HEADERS)(base + ((PIMAGE_DOS_HEADER)base)->e_lfanew);
    PIMAGE_EXPORT_DIRECTORY img_export_dir = (PIMAGE_EXPORT_DIRECTORY)(base + img_nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    DWORD* function_name_array =  (PDWORD)(base + img_export_dir->AddressOfNames);
    DWORD* function_addr_array =  (PDWORD)(base + img_export_dir->AddressOfFunctions);
    DWORD* function_ord_array  =  (PDWORD)(base + img_export_dir->AddressOfNameOrdinals);
    DWORD img_dir_export_size =   img_nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;

    if(!hModule || !ImportName)
        return NULL; // dalbajeb?

    for(DWORD i = 0; i < img_export_dir->NumberOfFunctions; i++) {
        char* fn_name = (char*)(base + function_name_array[i]);
        void* fn_addr = (void*)(base + function_addr_array[i]);

        if(((ULONG_PTR)fn_addr >= (ULONG_PTR)img_export_dir) && ((ULONG_PTR)fn_addr < ((ULONG_PTR)img_export_dir) + img_dir_export_size)) {
            char  fowarder_name[MAX_PATH] = { 0 };
            DWORD dot_offset = 0x00;
            char* fn_mod = NULL;
            char* pc_fn_name = NULL;

            Memcpy(fowarder_name, fn_addr, strlen((PCHAR)fn_addr));

            for (int i2 = 0; i2 < strlen((PCHAR)fowarder_name); i2++) {

                if (((PCHAR)fowarder_name)[i2] == '.') {
                    dot_offset = i2;
                    fowarder_name[i2] = '\0'; // crazy shenanigans
                    break;
                }
            }

            fn_mod	= fowarder_name;
            pc_fn_name	= fowarder_name + dot_offset + 1;

            fnLoadLibraryA pLoadLibraryA = (fnLoadLibraryA)get_proc_address(get_module_handle("kernel32"), "LoadLibraryA");
            if (pLoadLibraryA)
                return get_proc_address(pLoadLibraryA(fn_mod), pc_fn_name);
        }
        return (FARPROC)fn_addr;
    }

    return NULL;
}

HMODULE get_module_handle(const char* ModuleName) {
    PPEB peb = (PPEB)__readgsqword(0x60);
    PPEB_LDR_DATA ldr = (PPEB_LDR_DATA)(peb->LoaderData);
    PLDR_DATA_TABLE_ENTRY dte = (PLDR_DATA_TABLE_ENTRY)(ldr->InInitializationOrderModuleList.Flink);

    if(ModuleName == NULL)
        return (HMODULE)(dte->InInitializationOrderLinks.Flink);

    while(dte) {
        if(dte->FullDllName.Buffer && dte->FullDllName.Length < MAX_PATH) {
            char dllname[MAX_PATH] = { 0 };
            int index = 0;

            while(dte->FullDllName.Buffer[index]) {
                char buf = dte->FullDllName.Buffer[index];

                if(buf >= 'A' && buf <= 'Z')
                    dllname[index] = buf - 'A' - 'a';
                else
                    dllname[index] = buf;

                index++;
            }

            dllname[index] = '\0'; // nullterminatorenz

            if(dte->FullDllName.Buffer == dllname || dllname == ModuleName)
                return (HMODULE)(dte->InInitializationOrderLinks.Flink);
        }

        dte = *(PLDR_DATA_TABLE_ENTRY*)(dte);
    }

    return NULL;
}