#include "Hacking.h"

    bool Hacking::StartHook(HANDLE hProc, intptr_t modBaseAddr)
    {
        // setting Detour
        uintptr_t* srcAddr{ (uintptr_t*)(modBaseAddr + Offset::decAmmoInstruct) };
        constexpr uintptr_t srcSize{ 7 };
        const uintptr_t jmpBackAddr = (uintptr_t)srcAddr + srcSize;
        bool bDetour{ false };

        // alocating memory into targe process
        uintptr_t* gateway{ (uintptr_t*)VirtualAllocEx(
            hProc,
            NULL,
            MAX_PATH,
            MEM_COMMIT | MEM_RESERVE,
            PAGE_EXECUTE_READWRITE) };

        if (!gateway) return false;
            Hook::SetGatewayEx(
                hProc,
                gateway,
                (uintptr_t*)"\x90\x57\x8B\x7C\x24\x14\xE9",
                srcSize,
                jmpBackAddr);

        while (!GetAsyncKeyState(VK_DELETE) & 1)
        {
            if (GetAsyncKeyState(VK_F1) & 1)
                bDetour = !bDetour;

                if (bDetour)
                    Hook::DetourEx(hProc, srcAddr, gateway, srcSize);
                else
                    Hook::PatchEx(
                        hProc,
                        srcAddr,
                        (uintptr_t*)"\xFF\x0E\x57\x8B\x7C\x24\x14",
                        srcSize);

            Sleep(5);
        }

        CloseHandle(hProc);
        return true;
    }