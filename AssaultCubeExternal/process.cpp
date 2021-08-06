#include "process.h"

DWORD getProcIDByName(const wchar_t* procName)
{
	DWORD procID = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntryPoint;
		procEntryPoint.dwSize = sizeof(procEntryPoint);

		if (Process32First(hSnap, &procEntryPoint))
		{
			do
			{
				if (!_wcsicmp(procEntryPoint.szExeFile, procName)) 
				{
					procID = procEntryPoint.th32ProcessID;
					break;
				}
			} while(Process32Next(hSnap, &procEntryPoint));
		}
	}
	CloseHandle(hSnap);
	return procID;
}

uintptr_t getModuleBaseAddress(DWORD procID, const wchar_t* moduleName) 
{
	uintptr_t baseAddress = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);
	if (hSnap != INVALID_HANDLE_VALUE) 
	{
		MODULEENTRY32 moduleEntryPoint;
		moduleEntryPoint.dwSize = sizeof(moduleEntryPoint);
		if (Module32First(hSnap, &moduleEntryPoint))
		{
			do 
			{
				if (!_wcsicmp(moduleEntryPoint.szModule, moduleName))
				{
					baseAddress = (uintptr_t)moduleEntryPoint.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &moduleEntryPoint));
		}
	}
	CloseHandle(hSnap);
	return baseAddress;
}

uintptr_t FIndDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets) 
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}