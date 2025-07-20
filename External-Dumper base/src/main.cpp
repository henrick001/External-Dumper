#include <windows.h>
#include "mem/mem.h"
#include <iostream>
#include <print>
#include "update.h"
//MADE BY HENRICK
//THIS IS DETECTED BUT IDEALLY YOU WILL USE TS IN UR OWN GAME WHICH CANT GET YOU BANNED

uintptr_t PLACEID = 168556275; // i could chnage ts to use logs but its just extra work for no reason

BYTE* base_address = 0;
inline void attach() {
	DWORD pid;
	const char* roblox_name = "RobloxPlayerBeta.exe";
	HANDLE roblox_handle;
	uintptr_t base_addy;
	HWND targetWindow = FindWindowA(nullptr, "Roblox");
	GetWindowThreadProcessId(targetWindow, &pid);

	g_procHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	BYTE* base_add2 = memory->LocateModuleBase(pid, "RobloxPlayerBeta.exe");
	if (!base_add2) {
		CloseHandle(g_procHandle);
		std::println("[!] Failed to find Roblox base address, make sure Roblox is running dumbass.");
		std::cin.get();
		return;
	}
	std::println("[+] Found roblox base addy: 0x{:X}", reinterpret_cast<uintptr_t>(base_add2));
	base_address = base_add2;
	return;
}
uintptr_t getdatamodel() {
	uintptr_t fakeDataModel = memory->read<uintptr_t>(reinterpret_cast<uintptr_t>(base_address) + offsets::FakeDataModelPointer);
	return fakeDataModel + offsets::FakeDataModelToRealDatamodel;
} //theres 2 methods to get it if u wanna know the other one then lmk on discord @henrickiscool
//thats also the reason my dumper dumps 2 fake datamodel to real datamodel offsets


int main() {
	SetConsoleTitle("BASE BY HENRICK TS TUFF");
	attach();
	uintptr_t datamodel = getdatamodel();
	std::println("[+] Datamodel: 0x{:X}", datamodel); //# means 0x prefix X uppercase hex

	uintptr_t name_offset = 0x0;
	while (true)
	{
		std::string name = memory->readstring(memory->read<uintptr_t>(datamodel + name_offset));

		if (name == "Ugc" || name == "LuaApp")
		{
			std::println("[+] Name: {:#x}", name_offset);
			break;
		}

		name_offset++;
	}
	uintptr_t placeid_offset = 0x0;
	while (true) {
		uintptr_t placeid = memory->read<uintptr_t>(datamodel + placeid_offset);
		if (placeid == PLACEID) {
			std::println("[+] PlaceId: {:#x}", placeid_offset);
			break;
		}
		placeid_offset++;
	}

	uintptr_t childrenstart_offset = 0x0;
	while (true)
	{
		uintptr_t start = memory->read<uintptr_t>(datamodel + childrenstart_offset);
		uintptr_t possible_children = memory->read<uintptr_t>(start);

		for (int i = 0; i < 50; i++)
		{
			std::string name = memory->readstring(memory->read<uintptr_t>(memory->read<uintptr_t>(possible_children + i * 0x10) + name_offset));

			if (name == "Players" || name == "Workspace"
				|| name == "GuiService" || name == "LogService"
				|| name == "VideoCaptureService" || name == "Run Service")//all of ts is for speed
			{
				std::println("[+] ChildrenStart: {:#x}", childrenstart_offset);
				goto GOTCHILDRENSTART;
			}
		}


		childrenstart_offset += 1;
	}
	GOTCHILDRENSTART:;

	uintptr_t gameloaded_offset = 0x0;
	while (true)
	{
		int64_t loaded = memory->read<int64_t>(datamodel + gameloaded_offset);
		if (loaded == 31)
		{
			std::println("[+] GameLoaded: {:#x}", gameloaded_offset);
			break;
		}
		gameloaded_offset++;
	}
	uintptr_t classdesc_offset = 0x0;
	while (true) {
		std::string classname = memory->readstring(memory->read<uintptr_t>(memory->read<uintptr_t>(datamodel + classdesc_offset) + 0x8)); // 0x8 classname to thingy ud
		if (classname == "DataModel") {
			std::println("[+] ClassDescripter: {:#x}", classdesc_offset);
			break;
		}
		classdesc_offset++;
	}

	std::println("[+] ClassDescripterToClassName: 0x8");//hasnt changed in freaking 14 years

	/*
	For walkspeed just get humanoid then read and do like std::fabs(possible_walkspeed - actual_walkspeed) < 0.001f
	we do this because 0.001f is epilson and its used for like floating point numbers to compare. because
	obviously its extrememly difficult for floats to be the exact same we just check if they are similar
	up to a certain decimal point. this is called the epilson!!!!!

	I havent added more because my dumper is pretty hard to add only one offset of like it uses 
	classes which i dont wanna leak. if you really want me to add more offsets then let me know
	on discord @henrickiscool!!!!


	Anyways other than that you can just apply this concept to anything else!
	*/

	system("PAUSE");
	return 0;
}