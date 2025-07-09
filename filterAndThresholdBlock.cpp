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
    
    HANDLE hPipe = createReaderPipe(data_fliterPipe);
    if(hPipe == INVALID_HANDLE_VALUE )
    {
        // replace this with the logger process
        std::cerr << "Pipe cannot be made on the server side" << std::endl;
    }   
    BOOL connected = connectToClient(hPipe);
    if(connected)
    {
        std::cout << "Client Connected " << std::endl;
    }

while (1)
{
    uint8_t dataPt;
    BOOL success = readFromPipe(hPipe, &dataPt, 1);
    if (success)
    {
        std::lock_guard<std::mutex> lock(windowMutex);
        if (windowElements.size() == WINDOW_SIZE)
        {
            windowFull = true;
            windowElements.pop_back();
        }
        windowElements.insert(windowElements.begin(), dataPt);
    }
}


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
    std::thread t2(filteringThread, DEBUG_THRESHOLD_VALUE);
    t1.join();
    t2.join();
    
}