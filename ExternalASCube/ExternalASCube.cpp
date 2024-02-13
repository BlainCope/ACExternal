#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <vector>
#include <cstring>

auto GetModuleBaseAddress(DWORD dwProcId, const wchar_t* szModuleName) -> uintptr_t {
	uintptr_t dwModuleBaseAddress = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcId);
	if (hSnapshot != INVALID_HANDLE_VALUE) {
		MODULEENTRY32W ModuleEntry32; // Use MODULEENTRY32W for wide character support
		ModuleEntry32.dwSize = sizeof(MODULEENTRY32W);
		if (Module32FirstW(hSnapshot, &ModuleEntry32)) { // Use Module32FirstW for wide character support
			do {
				if (!wcscmp(ModuleEntry32.szModule, szModuleName)) {
					dwModuleBaseAddress = (uintptr_t)ModuleEntry32.modBaseAddr;
					break;
				}
			} while (Module32NextW(hSnapshot, &ModuleEntry32)); // Use Module32NextW for wide character support
		}
		CloseHandle(hSnapshot);
	}
	return dwModuleBaseAddress;
}

auto FindDMAAddy(HANDLE hProc, uintptr_t ptr, const std::vector<unsigned int>& offsets) -> uintptr_t {
	for (const auto& offset : offsets) {
		ReadProcessMemory(hProc, (BYTE*)ptr, &ptr, sizeof(ptr), nullptr);
		ptr += offset;
	}
	return ptr;
}

auto GetProcessId(const wchar_t* procName) -> DWORD {
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) return 0;

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	if (Process32First(hSnap, &pe32)) {
		do {
			if (!wcscmp(pe32.szExeFile, procName)) {
				CloseHandle(hSnap);
				return pe32.th32ProcessID;
			}
		} while (Process32Next(hSnap, &pe32));
	}
	CloseHandle(hSnap);
	return 0; // Return 0 if process not found
}

auto displayCurrentAmmoAndHealth(HANDLE hProcess, uintptr_t ammoAddr, uintptr_t healthAddr) -> void {
	int ammo = 0;
	int health = 0;
	ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammo, sizeof(ammo), nullptr);
	ReadProcessMemory(hProcess, (BYTE*)healthAddr, &health, sizeof(health), nullptr);
	std::cout << "Current ammo: " << ammo << std::endl;
	std::cout << "Current health: " << health << std::endl;
}

int main()
{
    HANDLE hProcess = 0;
    uintptr_t moduleBase = 0;
	uintptr_t dynamicPtrBaseAddr = 0;
	uintptr_t ammoAddr = 0;
	uintptr_t healthAddr = 0;

	DWORD procId = GetProcessId(L"ac_client.exe");

	if (procId)
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
		moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");
		dynamicPtrBaseAddr = moduleBase + 0x10F4F4;

		std::vector<unsigned int> ammoOffsets = { 0x374, 0x14, 0x0 };
		ammoAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffsets);

		std::vector<unsigned int> healthOffsets = { 0xF8 };
		healthAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, healthOffsets);
	}

	std::cout << "Welcome to External Assault Cube!" << std::endl;
	std::cout << "Press NUMPAD1 to set ammo to a whole number." << std::endl;
	std::cout << "Press NUMPAD2 to set health to a whole number." << std::endl;
	std::cout << "Press END to exit the program." << std::endl;
	std::cout << "If you need additional support contact me on Discord: *********#****." << std::endl;
	std::cout <<"----------------------------------------" << std::endl;

	while (TRUE)
	{
		if (GetAsyncKeyState(VK_END) & 1)
		{
			break;
		}

		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			int newAmmo = 0;
			std::cout << "Please enter the new ammo value: ";
			std::cin >> newAmmo;
			WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);
			displayCurrentAmmoAndHealth(hProcess, ammoAddr, healthAddr);
		}

		if (GetAsyncKeyState(VK_NUMPAD2) & 1)
		{
			int newHealth = 0;
			std::cout << "Please enter the new health value: ";
			std::cin >> newHealth;
			WriteProcessMemory(hProcess, (BYTE*)healthAddr, &newHealth, sizeof(newHealth), nullptr);
			displayCurrentAmmoAndHealth(hProcess, ammoAddr, healthAddr);
		}
	}

}
