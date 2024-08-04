#include "MemManager.h"

DWORD MemManager::GetProcId(const wchar_t* procName)
{
	DWORD procId{ 0 };

	HANDLE hSnap{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL) };

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (!_wcsicmp(procEntry.szExeFile, procName))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
	return procId;
}

HANDLE MemManager::GetHproc(DWORD procId)
{
	if (!procId) return false;
	HANDLE hProc{ OpenProcess(PROCESS_ALL_ACCESS, NULL, procId) };

	if (hProc == INVALID_HANDLE_VALUE) return false;
	const uintptr_t modBaseAddr{
		MemManager::GetModuleBaseAddr(
			procId,
			L"ac_client.exe") };

	return hProc;
}

uintptr_t MemManager::GetModuleBaseAddr(DWORD procId, const wchar_t* modName)
{
	uintptr_t modBaseAddr{ 0 };

	HANDLE hSnap{ CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId) };

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);

		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

uintptr_t MemManager::ReadMem(HANDLE hProc, uintptr_t baseAddr, std::vector<uintptr_t> offsets)
{
	uintptr_t dynamicMemAAddy{ baseAddr };
	
	for (uintptr_t i{ 0 }; i < offsets.size(); ++i)
	{
		dynamicMemAAddy += offsets[i];
		ReadProcessMemory(hProc, (BYTE*)dynamicMemAAddy, &dynamicMemAAddy, sizeof(dynamicMemAAddy), nullptr);
	}

	return dynamicMemAAddy;
}