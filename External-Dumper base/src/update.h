#include <windows.h>
namespace offsets {
	//THESE ARENT NEEDED ANYMORE THANKS TO RTTI
	inline constexpr uintptr_t FakeDataModelPointer = 0x6833728; //used for dumping fakedmtorealdm
	inline constexpr uintptr_t FakeDataModelToRealDatamodel = 0x1b8; //used for dumping fakedmtorealdm
}