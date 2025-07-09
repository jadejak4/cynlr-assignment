// already correct:
#include <iostream>
#include <chrono>
#include <thread>      // For std::thread
#include <windows.h>   // For Windows APIs
#include "common/commonArgs.h"
#include "common/sharedMemory.h"
#include "dataGenBlock.h"


#define MAX UINT8_MAX
#define MIN 0


// this can be redefined as a template.
inline uint8_t generateRandomNumber()
{   
    return static_cast<uint8_t>(MIN + std::rand() % (MAX - MIN + 1));
}

void randomNumberGen()
{
    while (1)
    {
        dataPt[0] = generateRandomNumber();
        dataPt[1] = generateRandomNumber();
    }
}


void sendDataToFilterBlock()
{
    uint8_t localBuffer[2];

    HANDLE hMap = createSharedMemory(data_fliterMemory, 1024);
    if (hMap == NULL || hMap == INVALID_HANDLE_VALUE)
    {
        std::cerr << "CreateFileMapping failed. Error: " << GetLastError() << std::endl;
        return;
    }

    void* ptr = mapSharedMemory(hMap, 1024);
    if (ptr == nullptr)
    {
        std::cerr << "MapViewOfFile failed. Error: " << GetLastError() << std::endl;
        CloseHandle(hMap);
        return;
    }

    while (true)
    {
        auto start = std::chrono::high_resolution_clock::now();
        localBuffer[0] = dataPt[0];
        localBuffer[1] = dataPt[1];
        writeToSharedMemory(ptr, localBuffer, sizeof(localBuffer));
        auto end = std::chrono::high_resolution_clock::now();  // End time
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "Iteration took: " << duration.count() << " nanoseconds" << std::endl;
    }

    unmapSharedMemory(ptr);
    closeSharedMemory(hMap);
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
    std::thread t1(randomNumberGen); // then start client

    t2.join();
    t1.join();

    return 0;
}