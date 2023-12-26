#include "header.h"

DWORD jmpBackAddr{NULL};

void __declspec(naked) NakedDstFunc()
{
    __asm
    {
        nop
        JMP jmpBackAddr
    }
}

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
            uintptr_t srcAddr{ (modBaseAddr + Offset::decAmmo)};
            constexpr uintptr_t srcSize{ 7 };
            jmpBackAddr = srcAddr + srcSize;
            uintptr_t stolenBytes{ NULL };
            bool bDetour{ false };

            // alocating memory into targe process
            uintptr_t* dst{ (uintptr_t*)VirtualAllocEx(
                hProc,
                NULL,
                MAX_PATH,
                MEM_COMMIT | MEM_RESERVE,
                PAGE_EXECUTE_READWRITE) };

            if(dst)
            {
                // writting shellcode into code cave
                WriteProcessMemory(hProc, dst, (uintptr_t*)"\x90\x57\x8B\x7C\x24\x14\xE9", srcSize, nullptr);
                // jump back address isnt the right one
                WriteProcessMemory(hProc, (uintptr_t*)((uintptr_t)dst + srcSize), &jmpBackAddr, sizeof(jmpBackAddr), nullptr);
            }
            else
            {
                return -1;
            }

            while (!GetAsyncKeyState(VK_DELETE) & 1)
            {
                if (GetAsyncKeyState(VK_F1) & 1)
                {
                    bDetour = !bDetour;

                    if (bDetour)
                    {
                        
                        stolenBytes = Hook::Detour(
                            hProc,
                            (uintptr_t*)srcAddr,
                            dst,
                            srcSize);
                    }
                    else
                    {
                        Hook::CloseDetour(hProc, (uintptr_t*)srcAddr, srcSize, stolenBytes);
                    }
                }
                Sleep(5);
            }
        }
        
        CloseHandle(hProc);
    }

    return 0;
}