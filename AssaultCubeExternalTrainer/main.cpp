#include "header.h"

int main()
{
    std::cout << "Assault Cube External Trainer by Kalvin \n";

    DWORD procId{ NULL };
    
    while (!procId)
    {
        procId = MemManager::GetProcId(L"ac_client.exe");
        Sleep(5);
    }

    if (procId)
    {
        HANDLE hProc{ OpenProcess(PROCESS_ALL_ACCESS, NULL, procId) };

        if (hProc != INVALID_HANDLE_VALUE)
        {
            const uintptr_t modBaseAddr{ MemManager::GetModuleBaseAddr(procId, L"ac_client.exe") };

            // adding the relative offset of the local player 
            uintptr_t localPlayer{ modBaseAddr + Offset::localPlayer };
            ReadProcessMemory(hProc, (BYTE*)localPlayer, &localPlayer, sizeof(localPlayer), nullptr);
            
            uintptr_t entityListAddr{ modBaseAddr + Offset::entityList };
            // adding the relative offset of the local player health
            uintptr_t lpHealth{ localPlayer + 0xF8 };

            // modifying the lp health memory
            constexpr int newHealth{ 1337 };
            WriteProcessMemory(hProc, (BYTE*)lpHealth, &newHealth, sizeof(newHealth), nullptr);
        }
        
        CloseHandle(hProc);
    }

    return 0;
}