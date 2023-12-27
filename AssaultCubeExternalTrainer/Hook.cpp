#include "header.h"

bool Hook::DetourEx(HANDLE hProc, uintptr_t* src, uintptr_t* dst, const uintptr_t srcSize)
{
	if (srcSize < 5) return false;

	// nopping bytes
	NopPatchEx(hProc, src, dst, srcSize);

	// setting page protection
	DWORD oldProtect{};
	VirtualProtectEx(hProc, src, srcSize, PAGE_EXECUTE_READWRITE, &oldProtect);

	// writting JMP opcode
	constexpr BYTE jmpInstruction{ 0xE9 };
	WriteProcessMemory(hProc, src, &jmpInstruction, sizeof(jmpInstruction), nullptr);

	// writting relative address
	const uintptr_t effectiveAddr{ ((uintptr_t)dst - (uintptr_t)src) - 5 };
	WriteProcessMemory(hProc, (uintptr_t*)((uintptr_t)src + 1), &effectiveAddr, sizeof(effectiveAddr), nullptr);

	VirtualProtectEx(hProc, src, srcSize, oldProtect, &oldProtect);

	return true;
}

void Hook::SetGatewayEx(HANDLE hProc, uintptr_t* src, uintptr_t* dst, const uintptr_t size, uintptr_t jmpBackAddr)
{
	// writting shellcode into the injected memory
	WriteProcessMemory(
		hProc,
		src,
		dst,
		size,
		nullptr);

	// jump back address isnt the right one
	WriteProcessMemory(
		hProc,
		(uintptr_t*)((uintptr_t)dst + size),
		&jmpBackAddr,
		sizeof(jmpBackAddr),
		nullptr);
}

void Hook::NopPatchEx(HANDLE hProc, uintptr_t* src, uintptr_t* dst, const uintptr_t size)
{
	uintptr_t* nopArray{ new uintptr_t[size] };

	memset(nopArray, 0x90, size);

	PatchEx(hProc, src, nopArray, size);

	delete[] nopArray;
}

void Hook::PatchEx(HANDLE hProc, uintptr_t* src, uintptr_t* dst,const uintptr_t size)
{
	// setting page protection
	DWORD oldProtect{};
	VirtualProtectEx(hProc, src, size, PAGE_EXECUTE_READWRITE, &oldProtect);

	// patching
	WriteProcessMemory(hProc, src, dst, size, nullptr);

	VirtualProtectEx(hProc, src, size, oldProtect, &oldProtect);
}