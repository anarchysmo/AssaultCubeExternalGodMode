#pragma once
#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <vector>

DWORD getProcIDByName(const wchar_t* procName);
uintptr_t getModuleBaseAddress(DWORD procID, const wchar_t* str);
uintptr_t FIndDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);