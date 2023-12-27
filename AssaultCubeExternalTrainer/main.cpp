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
            const uintptr_t modBaseAddr{
                MemManager::GetModuleBaseAddr(
                    procId,
                    L"ac_client.exe") 
            };

            // setting local player 
            uintptr_t localPlayer{ Entity::GetLocalPlayer(hProc, modBaseAddr)};
            
            // setting localplayer health member
            uintptr_t lpHealth{ localPlayer + 0xF8 };

            // LP health set
            constexpr int newHealth{ 1337 };
            WriteProcessMemory(hProc, (BYTE*)lpHealth, &newHealth, sizeof(newHealth), nullptr);

            //setting entityList
          //  uintptr_t entityListAddr{ modBaseAddr + Offset::entityList };
           // ReadProcessMemory(hProc, (BYTE*)entityListAddr, &entityListAddr, sizeof(entityListAddr), nullptr);

            // setting Detour
            uintptr_t* src{ (uintptr_t*)(modBaseAddr + Offset::decAmmo)};
            constexpr uintptr_t srcSize{ 7 };
            const uintptr_t jmpBackAddr = (uintptr_t)src + srcSize;
            uintptr_t* stolenBytes{ nullptr };
            bool bDetour{ false };

            // alocating memory into targe process
            uintptr_t* gateway{ (uintptr_t*)VirtualAllocEx(
                hProc,
                NULL,
                MAX_PATH,
                MEM_COMMIT | MEM_RESERVE,
                PAGE_EXECUTE_READWRITE) };

            if (gateway)
                Hook::SetGatewayEx(
                    hProc,
                    gateway,
                    (uintptr_t*)"\x90\x57\x8B\x7C\x24\x14\xE9",
                    srcSize,
                    jmpBackAddr);
            else
                return -1;

            while (!GetAsyncKeyState(VK_DELETE) & 1)
            {
                if (GetAsyncKeyState(VK_F1) & 1)
                {
                    bDetour = !bDetour;

                    if (bDetour)
                       Hook::DetourEx(hProc, src, gateway, srcSize);
                    else
                       Hook::PatchEx(
                           hProc,
                           src,
                           (uintptr_t*)"\xFF\x0E\x57\x8B\x7C\x24\x14",
                           srcSize);
                }
                Sleep(5);
            }

        }
        
        CloseHandle(hProc);
    }

    return 0;
}