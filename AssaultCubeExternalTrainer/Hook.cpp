#include "header.h"

uintptr_t Hook::Detour(HANDLE hProc, uintptr_t* src, uintptr_t* dst, const uintptr_t srcSize)
{
	uintptr_t stolenBytes{ NULL };

	if (srcSize < 5) return stolenBytes;

	// setting page protection
	DWORD oldProtect{};
	VirtualProtectEx(hProc, src, srcSize, PAGE_EXECUTE_READWRITE, &oldProtect);
	
	// setting stolen bytes
	uintptr_t srcCopy{};
	if (ReadProcessMemory(hProc, src, &srcCopy, sizeof(src), nullptr))
	{
		 stolenBytes = srcCopy;

		// writting JMP opcode
		constexpr BYTE jmpInstruction{ 0xE9 };
		WriteProcessMemory(hProc, src, &jmpInstruction, sizeof(jmpInstruction), nullptr);

		// writting relative address
		const uintptr_t effectiveAddr{ ((uintptr_t)dst - (uintptr_t)src) - 5 };
		WriteProcessMemory(hProc, (uintptr_t*)((uintptr_t)src+1), &effectiveAddr, sizeof(effectiveAddr), nullptr);

		VirtualProtectEx(hProc, src, srcSize, oldProtect, &oldProtect);
	}

	return stolenBytes;
}

void Hook::CloseDetour(HANDLE hProc, uintptr_t* src, const uintptr_t srcSize, uintptr_t stolenBytes)
{
	if(stolenBytes)
	{
		// setting page protection
		DWORD oldProtect{};
		VirtualProtectEx(hProc, src, srcSize, PAGE_EXECUTE_READWRITE, &oldProtect);

		WriteProcessMemory(hProc, src, &stolenBytes, sizeof(stolenBytes), nullptr);

		VirtualProtectEx(hProc, src, srcSize, oldProtect, &oldProtect);

		stolenBytes = NULL;
	}
}