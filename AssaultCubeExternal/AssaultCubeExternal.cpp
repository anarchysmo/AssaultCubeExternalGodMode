#include <iostream>
#include <windows.h>
#include <vector>
#include "process.h"

int main()
{
    //get process ID by name
    DWORD procID = getProcIDByName(L"ac_client.exe");

    //get module base address
    uintptr_t baseAddr = getModuleBaseAddress(procID, L"ac_client.exe");
    //get proc handle 
    HANDLE hProc = 0;
    hProc = OpenProcess(PROCESS_ALL_ACCESS, NULL, procID);

    std::cout << "module handle address: " << "0x" << std::hex << baseAddr << std::endl;

    //get base arddr of pointer chain
    uintptr_t dynamicPtrBaseAddr = baseAddr + 0x10F4F4;
    std::vector<unsigned int> offsets = { 0xf8 };
    uintptr_t healthAddr = FIndDMAAddy(hProc, dynamicPtrBaseAddr, offsets);

    std::cout << "health address: " << "0x" << std::hex << healthAddr << std::endl;

    //do bad-boy stuff
    int healthValue = 0;
    while (true) 
    {
        ReadProcessMemory(hProc, (BYTE*)healthAddr, &healthValue, sizeof(healthValue), nullptr);
        if (GetAsyncKeyState(VK_END) & 1) 
        {
            break;
        }
        if (healthValue < 100) 
        {
            int newHealthValue = 100;
            WriteProcessMemory(hProc, (BYTE*)healthAddr, &newHealthValue, sizeof(newHealthValue), nullptr);
            std::cout << "changed value to 100" << std::endl;
        }
        Sleep(1);
    }

    return 0;
}
