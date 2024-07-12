#include <Windows.h>

#include <stdio.h>
#include <wchar.h>

#define WINDOWS_INCLUDE
#include "../../../../../decompile/General/AltMods/OnlineCTR/global.h"

DWORD WINAPI Mod_Entry(HMODULE hModule);
int normalMain();
void Stage2();
HMODULE hmodule;

extern char* pBuf;
extern struct OnlineCTR* octr;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ui_reason_for_call, LPVOID lpReserved)
{
	hmodule = hModule;
	switch (ui_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Mod_Entry, hModule, 0, 0));
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

DWORD WINAPI Mod_Entry(HMODULE hModule)
{
	while (1)
	{
		int pid = GetCurrentProcessId();

		char duckName[100];
		sprintf_s(duckName, 100, "duckstation_%d", pid);

		TCHAR duckNameT[100];
		swprintf(duckNameT, 100, L"%hs", duckName);

		// 8 MB RAM
		const unsigned int size = 0x800000;
		HANDLE hFile = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, duckNameT);
		pBuf = (char*)MapViewOfFile(hFile, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, size);

		if (pBuf != 0)
		{
			octr = (struct OnlineCTR*)&pBuf[0x8000C000 & 0xffffff];
			Stage2();
		}

		Sleep(1000);
	}
}

void Stage2()
{
	while (1)
	{
		if (octr->IsBootedPS1 != 0)
			normalMain();

		Sleep(100);
	}
}