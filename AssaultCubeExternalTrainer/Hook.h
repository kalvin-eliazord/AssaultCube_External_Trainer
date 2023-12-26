#pragma once
namespace Hook
{
	uintptr_t Detour(HANDLE hProc, uintptr_t* src, uintptr_t* dst, const uintptr_t size);
	void CloseDetour(HANDLE hProc, uintptr_t* src, const uintptr_t size, uintptr_t stolenBytes);
};