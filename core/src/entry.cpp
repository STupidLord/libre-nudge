// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#include <windows.h>

BOOL WINAPI DllMain(
	HINSTANCE instance_handle,
	DWORD reason,
	LPVOID reserved) {
    // DisableThreadLibraryCalls(instance_handle);
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	} // Don't necessarily need a full switch here, but :shrug:
	return TRUE;
}
