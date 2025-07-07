// already correct:
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <climits>
#include <chrono>
#include <thread>      // For std::thread
#include <windows.h>   // For Windows APIs
#include "common/commonArgs.h"

#define MAX UINT8_MAX
#define MIN 0

uint8_t generateRandomNumber()
{
    
    return static_cast<uint8_t>(MIN + std::rand() % (MAX - MIN + 1));
}

// try using function templates or pointers
void randomNumberGen1()
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

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to connect to pipe. Error: " << GetLastError() << "\n";
        return;
    }

    DWORD bytesWritten;
    while (1)
    {
        uint8_t dataPt = generateRandomNumber();

        // You must pass the address of dataPt, not the value
        BOOL success = WriteFile(hPipe, &dataPt, 1, &bytesWritten, NULL);
        if (!success)
        {
            std::cerr << "Write has failed. Error: " << GetLastError() << "\n";
            break;  // Stop on write failure
        }
    }

    CloseHandle(hPipe);
}

// try using function templates or pointers
void randomNumberGen2()
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
    
    if(hPipe == INVALID_HANDLE_VALUE )
    {
        // replace this with the logger process
        std::cerr << "Pipe cannot be made on the server side" << std::endl;
    }
    BOOL connected = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
    std::cout << "Client connected!\n";
    while(1)
    {
        if(connected)
        {
            uint8_t dataPt;
            DWORD bytesRead;

            BOOL success = ReadFile(hPipe, &dataPt, sizeof(dataPt), &bytesRead, NULL);
            if(success && bytesRead)
            {
                std::cout << static_cast<int>(dataPt) << std::endl;
            }
            else{
                std::cerr << "file read has failed " << std::endl; 
            }
        }
    }
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
    return;
}


int main()
{
    std::srand(static_cast<unsigned int>(std::time(0)));

    std::thread t2(randomNumberGen2); // start server first
    std::this_thread::sleep_for(std::chrono::milliseconds(1)); // wait for pipe to be created
    std::thread t1(randomNumberGen1); // then start client

    t2.join();
    t1.join();

    return 0;
}