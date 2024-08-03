#include <iostream>
#include "Hacking.h"

int main()
{
    std::cout << "Assault Cube External Corty Trainer \n";

    const wchar_t* procName{ L"ac_client.exe" };
    const DWORD procId{ MemManager::GetProcId(procName) };
    Hacking hacking;
    if (!hacking.StartHook(MemManager::GetHproc(procId), MemManager::GetModuleBaseAddr(procId, procName)))
    {
        std::cerr << "Hooking failed !\n";
        return -1;
    }

    return 0;
}