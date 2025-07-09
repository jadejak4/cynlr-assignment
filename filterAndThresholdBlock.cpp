#include "filterAndThresholdBlock.h"
#include "common/sharedMemory.h"
#include "common/commonArgs.h"
#include <vector>
#include <thread>
#include <iostream>
#include <algorithm>

inline double filteredOutput(const std::vector<uint8_t> values)
{
    double multipliedVal = 0;
    for(int i = 0; i < WINDOW_SIZE; i++)    
    {
        multipliedVal += double(values[i]) * filterCofficients[i];
    }

    return multipliedVal;
}


// we can make it so that 
void filteringThread(int thresholdValue)
{
    while (1)
    {
        if(windowFull)
        {
            std::vector<uint8_t> localCopy;
            {   
                std::lock_guard<std::mutex> lock(windowMutex);  
                localCopy = windowElements;
            }
            double res = filteredOutput(localCopy);
            if(res < thresholdValue)
            {
                // output true;
            }
            else
            {
                //output false;
            }
        }   
}
        
}

void slidingWindowThread()
{
    HANDLE hMap = openSharedMemory(data_fliterMemory);
    if (hMap == NULL || hMap == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Memory cannot be accessed from the filterSide. Error: " << GetLastError() << std::endl;
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
        uint8_t dataPt;  // Must match writer’s 2-byte write
        bool success = readFromSharedMemory(ptr, &dataPt, sizeof(dataPt));
        if (success)
        {
            std::lock_guard<std::mutex> lock(windowMutex);
            if (windowElements.size() == WINDOW_SIZE)
            {
                windowFull = true;
                windowElements.pop_back();
            }
                windowElements.insert(windowElements.begin(), dataPt);
            // Print window
            for (int i = 0; i < windowElements.size(); ++i)
            {
                std::cout << static_cast<int>(windowElements[i]) << " ";
            }
            std::cout << std::endl;
        }

    }

    unmapSharedMemory(ptr);
    closeSharedMemory(hMap);
}

int main(int argc, char* argv[])
{
    // int thresholdValue;
    // if(argc < 1)
    // {
    //     thresholdValue = DEBUG_THRESHOLD_VALUE;

    // }
    // else
    // {
    //     thresholdValue = argv[1];
    // }

    // added for testing
    // uint8_t testValues[WINDOW_SIZE] = {23,156, 223, 2, 67, 79, 110, 128, 208}; 
    std::thread t1(slidingWindowThread);
    // std::thread t2(filteringThread, DEBUG_THRESHOLD_VALUE);
    t1.join();
    // t2.join();
    
}