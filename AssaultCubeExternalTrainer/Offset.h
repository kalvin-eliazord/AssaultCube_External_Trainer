#pragma once
#include <Windows.h>

namespace Offset
{
	constexpr DWORD localPlayer{ 0x10f4f4 };
	constexpr intptr_t entityList{ 0x10f4f8 };
	constexpr intptr_t decAmmoInstruct{ 0x637E9 };
	intptr_t modBaseAddr{};
	namespace Entity {
		const DWORD health{ 0xf8 };
	}


};