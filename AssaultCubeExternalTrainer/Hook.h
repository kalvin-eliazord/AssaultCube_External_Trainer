#pragma once
#include <Windows.h>

namespace Hook
{
	bool DetourEx(HANDLE hProc, uintptr_t* src, uintptr_t* dst, const uintptr_t size);
	void SetGatewayEx(HANDLE hProc, uintptr_t* src, uintptr_t* dst, const uintptr_t size, uintptr_t jmpBackAddr);
	void NopPatchEx(HANDLE hProc, uintptr_t* src, uintptr_t* dst, const uintptr_t srcSize);
	void PatchEx(HANDLE hProc, uintptr_t* src, uintptr_t* dst, const uintptr_t size);
};