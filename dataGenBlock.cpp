// already correct:
#include <iostream>

#include <chrono>
#include <thread>      // For std::thread
#include <windows.h>   // For Windows APIs
#include "common/commonArgs.h"
#include "common/namedPipes.h"


#define MAX UINT8_MAX
#define MIN 0

uint8_t generateRandomNumber()
{
    
    return static_cast<uint8_t>(MIN + std::rand() % (MAX - MIN + 1));
}

// try using function templates or pointers
void randomNumberGen1()
{
    HANDLE hPipe = createWriterPipe();

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to connect to pipe. Error: " << GetLastError() << "\n";
        return;
    }
    while (1)
    {
        uint8_t dataPt = generateRandomNumber();

        BOOL success = writeToPipe(hPipe, &dataPt, 1);
        if(!success)
        {
            std::cerr << "error writing to the pipe " << std::endl;
        }
    }

    CloseHandle(hPipe);
}

// try using function templates or pointers
void randomNumberGen2()
{
    HANDLE hPipe = createReaderPipe();
    if(hPipe == INVALID_HANDLE_VALUE )
    {
        // replace this with the logger process
        std::cerr << "Pipe cannot be made on the server side" << std::endl;
    }
    BOOL connected = connectToClient(hPipe);
    std::cout << "Client connected!\n";
    while(1)
    {
        if(connected)
        {
            uint8_t dataPt;
            DWORD bytesRead;
            BOOL success = readFromPipe(hPipe, &dataPt, 1);

            if(!success)
            {
                std::cerr << "file read has failed " << std::endl; 
            }
        }
        else
        {
            std::cerr << "Pipe is disconnected " << std::endl;
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