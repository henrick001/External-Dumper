#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <psapi.h>
#include <memory>

#pragma comment(lib, "psapi")
using std::string;
inline HANDLE g_procHandle = nullptr;

class mem
{
public:
    template <typename T>
    T read(uintptr_t address) {
        T buffer{};
        SIZE_T bytesRead = 0;
        BOOL success = ReadProcessMemory(g_procHandle, reinterpret_cast<LPCVOID>(address), &buffer, sizeof(T), &bytesRead);
        if (!success || bytesRead != sizeof(T)) {
            DWORD error = GetLastError();
            return T{};
        }
        return buffer;
    }
    template <typename T>
    inline bool write(uintptr_t address, const T& value) {
        SIZE_T bytesWritten = 0;
        BOOL success = WriteProcessMemory(g_procHandle, reinterpret_cast<LPVOID>(address), &value, sizeof(T), &bytesWritten);
        if (!success || bytesWritten != sizeof(T)) {
            DWORD error = GetLastError();
            return false;
        }
        return true;
    }

    string readstring(uintptr_t addr)
    {
        string result;
        result.reserve(204);

        const int length = read<int>(addr + 0x18);
        const uintptr_t stringAddr = length >= 16 ? read<uintptr_t>(addr) : addr;

        int offset = 0;
        while (offset < 200)
        {
            char ch = read<char>(stringAddr + offset);
            if (ch == '\0') break;

            result.push_back(ch);
            offset += sizeof(char);
        }

        return result;
    }

    string readstring2(uintptr_t addr)
    {
        const int length = read<int>(addr + 0x18);
        if (length >= 16)
        {
            const uintptr_t heapPtr = read<uintptr_t>(addr);
            return readstring(heapPtr);
        }

        return readstring(addr);
    }
    BYTE* LocateModuleBase(DWORD pid, const char* modulename);

    uintptr_t GetModuleBase(HANDLE processHandle, string& sModuleName);

};
const auto memory = std::make_unique<mem>();