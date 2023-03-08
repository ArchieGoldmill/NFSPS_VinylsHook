#include "includes/injector/injector.hpp"

auto sub_76F800 = (int(__thiscall*)(void*, int, int))0x0076F800;
auto UpperStringHash = (int(__cdecl*)(const char*))0x004366E0;

int __fastcall sub_76F800_Hook(void* _this, int param, int a2, int a3)
{
	int result = sub_76F800(_this, a2, a3);
	if (!result)
	{
		result = sub_76F800(_this, UpperStringHash("is350"), UpperStringHash("default_is350"));
	}

	return result;
}

int __fastcall VinylsHook(void* _this, int param, int a2, int a3)
{
	auto isDecal = (int*)0x00B32D64;
	return *isDecal;
}

void Init()
{
	injector::MakeCALL(0x007E9C9D, VinylsHook, true);
	injector::MakeCALL(0x007E9DE3, VinylsHook, true);

	injector::WriteMemory<unsigned short>(0x007CE6A0, 0x01B0, true);
	injector::WriteMemory<unsigned short>(0x007CE58D, 0x01B0, true);
	injector::WriteMemory<unsigned short>(0x007CE5D2, 0xC031, true);
	injector::MakeNOP(0x007CE5FB, 0x1C, true);

	injector::WriteMemory<unsigned char>(0x007DC3CF, 0xEB, true);
	injector::MakeNOP(0x007DC2FB, 2, true);

	injector::MakeCALL(0x0076F95A, sub_76F800_Hook, true);
	injector::MakeCALL(0x0076F98F, sub_76F800_Hook, true);
	injector::MakeCALL(0x0076FA9A, sub_76F800_Hook, true);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
		IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)(base);
		IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);

		if ((base + nt->OptionalHeader.AddressOfEntryPoint + (0x400000 - base)) == 0x00828C25) // Check if .exe file is compatible - Thanks to thelink2012 and MWisBest
			Init();

		else
		{
			MessageBoxA(NULL, "This .exe is not supported.\nPlease use v1.1 NFS.exe from BATTERY (27,4 MB (28.739.656 bytes)).", "NFSPS - Vinyls Hook", MB_ICONERROR);
			return FALSE;
		}
	}
	return TRUE;
}

