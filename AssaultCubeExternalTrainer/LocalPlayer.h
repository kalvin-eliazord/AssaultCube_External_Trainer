#pragma once
#include "Offset.h"
#include <cstdint>

struct LocalPlayer
{
	intptr_t GetLocalPlayer(HANDLE hProc, uintptr_t modBaseAddr);
};