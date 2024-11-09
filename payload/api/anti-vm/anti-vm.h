#ifndef PAYLOAD_ANTI_VM_H
#define PAYLOAD_ANTI_VM_H

#include <windows.h>
#include <cstring>

BOOL check_registry(HKEY hKey, char* reg_key);
BOOL check_regkey_value_str(HKEY hKey, char* reg_key, char* value, char* lookup);
BOOL check_file_exist(char* file_name);
BOOL check_mac_vendor(char* mac);
BOOL check_adapter_name(char* adapter_name);



#endif
