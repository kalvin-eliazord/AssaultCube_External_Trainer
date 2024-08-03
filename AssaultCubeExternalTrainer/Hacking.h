#pragma once
#include "MemManager.h"
#include "Entity.h"
#include <iostream>
#include "Offset.h"
#include "Hook.h"

struct Hacking
{
	bool StartHook(HANDLE hProc, intptr_t modBaseAddr);
};