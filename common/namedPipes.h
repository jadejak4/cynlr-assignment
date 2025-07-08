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

BOOL writeToPipe(HANDLE hPipe, uint8_t* dataPt, int len)
{
    DWORD bytesWritten;
    BOOL success = WriteFile(hPipe, dataPt, len, &bytesWritten, NULL);
    if(!success)
    {
        std::cerr << "write has failed " << std::endl;
    }
    return success;
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
    DWORD bytesRead;

    BOOL success = ReadFile(hPipe, dataPt, len, &bytesRead, NULL);
    if(!success)
    {
        std::cerr << "file read has failed " << std::endl; 
    }
    return success;
}