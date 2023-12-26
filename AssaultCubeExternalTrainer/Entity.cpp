#include "header.h"

uintptr_t Entity::GetLocalPlayer(HANDLE hProc, uintptr_t modBaseAddr)
{
    // setting local player 
    uintptr_t localPlayer{ modBaseAddr + Offset::localPlayer };
    ReadProcessMemory(hProc, (BYTE*)localPlayer, &localPlayer, sizeof(localPlayer), nullptr);
    
    return localPlayer;
}