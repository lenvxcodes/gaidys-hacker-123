cmake_minimum_required(VERSION 3.30)
project(payload C)

set(CMAKE_C_STANDARD 23)

add_executable(payload main.c
        api/anti-vm/anti-vm.h
        api/discord_api/http.h
        api/discord_api/http.c
        api/ntshit/structs.h
        api/utils/hashfuncs.h
        api/utils/hashfuncs.c
        api/utils/fnPtrs.h)
