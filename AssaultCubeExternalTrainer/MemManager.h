#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

namespace MemManager
{
	HANDLE hProc{};
	DWORD GetProcId(const wchar_t* procName);
	HANDLE GetHproc(DWORD procId);
	uintptr_t GetModuleBaseAddr(DWORD procId, const wchar_t* modName);
	uintptr_t ReadMem(HANDLE hProc, uintptr_t baseAddr, std::vector<uintptr_t> offsets);
};