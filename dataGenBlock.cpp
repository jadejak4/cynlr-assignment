// already correct:
#include <iostream>
#include <chrono>
#include <thread>      // For std::thread
#include <windows.h>   // For Windows APIs
#include "common/commonArgs.h"
#include "common/namedPipes.h"
#include "dataGenBlock.h"


#define MAX UINT8_MAX
#define MIN 0

uint8_t generateRandomNumber()
{
    
    return static_cast<uint8_t>(MIN + std::rand() % (MAX - MIN + 1));
}

// try using function templates or pointers
void randomNumberGen1()
{
    HANDLE hPipe = createWriterPipe(internalPipeName);

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to connect to pipe. Error: " << GetLastError() << "\n";
        return;
    }
    while (1)
    {
        uint8_t dataPt[2] = { generateRandomNumber(), generateRandomNumber() };

        BOOL success = writeToPipe(hPipe, dataPt, 2);
        if(!success)
        {
            std::cerr << "error writing to the pipe " << std::endl;
        }
    }
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
}


void sendDataToFilterBlock()
{
    HANDLE writerPipe = createWriterPipe(data_fliterPipe);
    HANDLE readerPipe = createReaderPipe(internalPipeName);
    if(readerPipe == INVALID_HANDLE_VALUE || writerPipe == INVALID_HANDLE_VALUE)
    {
        std::cerr << "pipe handles are invalid " << std::endl;
    }
    BOOL connected = connectToClient(readerPipe);
    while(1)
    {
        if(connected)
        {
            uint8_t dataPt[2];
            BOOL success = readFromPipe(readerPipe, dataPt, 2);
            if(success)
            {
                BOOL success = writeToPipe(writerPipe, dataPt, 2);
            }
        }
    }

}


int main(int argc, char *argv[])
{

    if(argc < 1)
    {
        // this means we are manually generating the random numbers
    }
    std::srand(static_cast<unsigned int>(std::time(0)));
    // starting the thread for receiving the data
    std::thread t2(sendDataToFilterBlock);
    std::this_thread::sleep_for(std::chrono::milliseconds(1)); // wait for pipe to be create
    std::thread t1(randomNumberGen1); // then start client

    t2.join();
    t1.join();

    return 0;
}