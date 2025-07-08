#include "filterAndThresholdBlock.h"
#include "common/namedPipes.h"
#include "common/commonArgs.h"


double filteredOutput(const uint8_t *values, int len)
{
    double multipliedVal = 0;
    if(len != WINDOW_SIZE)
        return double(0);
    for(int i = 0; i < WINDOW_SIZE; i++)
    {
        multipliedVal += double(values[i]) * filterCofficients[i];
    }

    return multipliedVal;
}

// void filteringThread(int thresholdValue)
// {
//     if(windowFull)
//     {
//         filteredOutput   
//     }
    
// }

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

    while(1)
    {
        uint8_t dataPt;
        BOOL success = readFromPipe(hPipe, &dataPt, 1);
        if(success)
        {
            if (windowElements.size() == WINDOW_SIZE)
            {
                windowElements.pop_back();  // remove last
            }
            windowElements.push_front(dataPt); // add new at the front
            for(int i = 0; i < windowElements.size(); i++)
            {
                std::cout << static_cast<int>(windowElements[i]) << " ";
            }
            std::cout << "\n" ;
            // windowFull = true;
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
    t1.join();
    
}