#include "LocalPlayer.h"

intptr_t LocalPlayer::GetLocalPlayer(HANDLE hProc, uintptr_t modBaseAddr)
{
    static intptr_t localPlayer{ modBaseAddr + Offset::localPlayer };
    ReadProcessMemory(hProc, (BYTE*)localPlayer, &localPlayer, sizeof(localPlayer), nullptr);
    return localPlayer;
}