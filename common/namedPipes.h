#pragma once 
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <climits>
#include <iostream>
#include <stdio.h>
#include <thread>      // For std::thread
#include <windows.h>   // For Windows APIs

HANDLE createWriterPipe(const char* pipeName)
{
    HANDLE hPipe;
    hPipe = CreateFileA(
        pipeName,             // pipe name
        GENERIC_WRITE,        // write access
        0,                    // no sharing
        NULL,                 // default security attributes
        OPEN_EXISTING,        // opens existing pipe
        0,                    // default attributes
        NULL);                // no template file

    return hPipe;

}

inline BOOL writeToPipe(HANDLE hPipe, uint8_t* dataPt, int len)
{
    DWORD bytesWritten = 0;
    BOOL success = WriteFile(hPipe, dataPt, len, &bytesWritten, NULL);

    if (!success)
    {
        DWORD err = GetLastError();
        if (err != ERROR_IO_PENDING)
        {
            std::cerr << "WriteFile failed. Error: " << err << "\n";
        }
    }
    else if (bytesWritten != len)
    {
        std::cerr << "Warning: Partial write — wrote " << bytesWritten << " out of " << len << "\n";
    }

    return success && (bytesWritten == len);
}

HANDLE createReaderPipe(const char* pipeName)
{
        
    HANDLE hPipe = CreateNamedPipeA(
        pipeName,                  // name of the pipe
        PIPE_ACCESS_DUPLEX,       // read/write access
        PIPE_TYPE_MESSAGE |       // message-type pipe
        PIPE_READMODE_MESSAGE |   // message read mode
        PIPE_WAIT,                // blocking mode
        1,                        // max. instances
        1024,                     // output buffer size
        1024,                     // input buffer size
        0,                        // client time-out
        NULL);                    // default security attribute
    return hPipe;
}

BOOL connectToClient(HANDLE hPipe)
{
    return ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
}

BOOL readFromPipe(HANDLE hPipe, uint8_t* dataPt, int len)
{
    DWORD bytesRead = 0;
    BOOL success = ReadFile(hPipe, dataPt, len, &bytesRead, NULL);

    if (!success)
    {
        DWORD err = GetLastError();
        if (err != ERROR_MORE_DATA && err != ERROR_IO_PENDING)
        {
            std::cerr << "ReadFile failed. Error: " << err << "\n";
        }
    }
    else if (bytesRead != len)
    {
        std::cerr << "Warning: Partial read — got " << bytesRead << " bytes out of " << len << "\n";
    }

    return success && (bytesRead == len);
}