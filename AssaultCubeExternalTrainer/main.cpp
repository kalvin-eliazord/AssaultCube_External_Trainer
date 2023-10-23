#include "header.h"

int main()
{
    std::cout << "Assault Cube External Trainer by Kalvin\n";

    DWORD procId{ MemManager::GetProcId(L"ac_client.exe") };

    HANDLE hProc{ OpenProcess(PROCESS_ALL_ACCESS, NULL, procId) };

    const uintptr_t modBaseAddr{ MemManager::GetModuleBaseAddr(procId, L"ac_client.exe")};

    // adding the relative offset of the local player 
    uintptr_t localPlayer{ modBaseAddr + 0x10f4f4 };

    ReadProcessMemory(hProc, (BYTE*)localPlayer, &localPlayer, sizeof(localPlayer), nullptr);

    // adding the relative offset of the local player health
    uintptr_t lpHealth{ localPlayer + 0xF8 };

    // modifying the lp health memory
    constexpr int newHealth{ 1337 };
    WriteProcessMemory(hProc, (BYTE*)lpHealth, &newHealth, sizeof(newHealth), nullptr);

    CloseHandle(hProc);
    return 0;
}