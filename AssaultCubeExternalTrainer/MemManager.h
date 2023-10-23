#pragma once
namespace MemManager
{
	DWORD GetProcId(const wchar_t* procName);
	uintptr_t GetModuleBaseAddr(DWORD procId, const wchar_t* modName);
	uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t baseAddr, std::vector<uintptr_t> offsets);
};