#pragma once

#include <windows.h>
#include <iostream>
#include <string>
#include <cstdint>

inline HANDLE createSharedMemory(const char* name, size_t size)
{
    HANDLE hMapFile = CreateFileMappingA(
        INVALID_HANDLE_VALUE,    // Use system paging file
        NULL,                    // Default security
        PAGE_READWRITE,          // Read/write access
        0,                       // Max size (high-order DWORD)
        static_cast<DWORD>(size),// Max size (low-order DWORD)
        name                     // Name of shared memory object
    );

    if (hMapFile == NULL)
    {
        std::cerr << "CreateFileMapping failed. Error: " << GetLastError() << "\n";
    }

    return hMapFile;
}

inline HANDLE openSharedMemory(const char* name)
{
    HANDLE hMapFile = OpenFileMappingA(
        FILE_MAP_ALL_ACCESS,  // Read/write access
        FALSE,                // Do not inherit handle
        name                  // Name of shared memory object
    );

    if (hMapFile == NULL)
    {
        std::cerr << "OpenFileMapping failed. Error: " << GetLastError() << "\n";
    }

    return hMapFile;
}

inline void* mapSharedMemory(HANDLE hMapFile, size_t size)
{
    LPVOID pBuf = MapViewOfFile(
        hMapFile,             // Handle to map object
        FILE_MAP_ALL_ACCESS,  // Read/write access
        0,
        0,
        size
    );

    if (pBuf == NULL)
    {
        std::cerr << "MapViewOfFile failed. Error: " << GetLastError() << "\n";
    }

    return pBuf;
}

inline void unmapSharedMemory(void* ptr)
{
    if (!UnmapViewOfFile(ptr))
    {
        std::cerr << "UnmapViewOfFile failed. Error: " << GetLastError() << "\n";
    }
}

inline void closeSharedMemory(HANDLE hMapFile)
{
    if (!CloseHandle(hMapFile))
    {
        std::cerr << "CloseHandle failed. Error: " << GetLastError() << "\n";
    }
}

inline bool writeToSharedMemory(void* memory, const uint8_t* data, size_t len)
{
    if (!memory || !data)
        return false;

    memcpy(memory, data, len);
    return true;
}

inline bool readFromSharedMemory(const void* memory, uint8_t* outData, size_t len)
{
    if (!memory || !outData)
        return false;

    memcpy(outData, memory, len);
    return true;
}
