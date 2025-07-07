#include "filterAndThresholdBlock.h"

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

void filteringThread(int thresholdValue)
{
    if(windowFull)
    {
        filteredOutput   
    }
    
}

void slidingWindowThread()
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
    BOOL connected;
    while(!connected)
    {
        connected = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

    }
    std::cout << "Client connected" << std::endl;

    while(1)
    {
        uint8_t dataPt;
        DWORD bytesRead;
        BOOL success = ReadFile(hPipe, &dataPt, sizeof(dataPt), &bytesRead, NULL);
        if(success && bytesRead)
        {
            if (windowElements.size() == WINDOW_SIZE)
            {
                windowElements.pop_back();  // remove last
            }
            windowElements.push_front(dataPt); // add new at the front
            windowFull = true;
        }

    }


}


int main(int argc, char* argv[])
{
    int thresholdValue;
    if(argc < 1)
    {
        thresholdValue = DEBUG_THRESHOLD_VALUE;

    }
    else
    {
        thresholdValue = argv[1];
    }
    uint8_t testValues[WINDOW_SIZE] = {23,156, 223, 2, 67, 79, 110, 128, 208}; 
    std::cout << double(filteredOutput(testValues, WINDOW_SIZE)) << std::endl;
    
}