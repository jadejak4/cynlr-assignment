#include "filterAndThresholdBlock.h"
#include "common/sharedMemory.h"
#include "common/commonArgs.h"
#include <vector>
#include <thread>
#include <iostream>
#include <algorithm>

inline double filteredOutput(const std::vector<uint8_t> &values)
{
    double multipliedVal = 0;
    for(int i = 0; i < WINDOW_SIZE; i++)    
    {
        multipliedVal += double(values[i]) * filterCofficients[i];
    }

    return multipliedVal;
}


void filteringThread(double thresholdValue)
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

            // added just for debugging
            // for (int i = 0; i < localCopy.size(); i++)
            // {
            //     std::cout << static_cast<int>(localCopy[i]) << " ";
            // }
            // std::cout << res;
            // std::cout << "\n";
            // this window has been processed. We do not want to porcess the same data again
            windowFull = false;
            if(res >= thresholdValue)
            {
                // output true to the next block
            }
            else
            {
                // output false to the next block
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
    if (windowFull)
        continue; // wait for filter to consume it

    uint8_t dataPt;
    bool success = readFromSharedMemory(ptr, &dataPt, sizeof(dataPt));
    if (success)
    {
        std::lock_guard<std::mutex> lock(windowMutex);
        if (windowElements.size() == WINDOW_SIZE)
        {
            windowElements.pop_back();
        }
        windowElements.insert(windowElements.begin(), dataPt);

        if (windowElements.size() == WINDOW_SIZE)
        {
            windowFull = true;
        }
    }
}

    unmapSharedMemory(ptr);
    closeSharedMemory(hMap);
}

int main(int argc, char* argv[])
{
    // this is added to take in the thresholdValue
    double thresholdValue;
    if(argc < 2)
    {
        thresholdValue = DEBUG_THRESHOLD_VALUE;

    }
    else
    {
        thresholdValue = std::stod(argv[1]);
    }
 
    std::thread t1(slidingWindowThread);
    std::thread t2(filteringThread, thresholdValue);
    t1.join();
    t2.join();
    
}