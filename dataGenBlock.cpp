#include "common/commonArgs.h"
#include "common/sharedMemory.h"
#include "common/helperFunctions.h"
#include "dataGenBlock.h"

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
        localBuffer[0] = dataPt[0];
        localBuffer[1] = dataPt[1];
        writeToSharedMemory(ptr, localBuffer, sizeof(localBuffer));
    }

    unmapSharedMemory(ptr);
    closeSharedMemory(hMap);
}


int main(int argc, char *argv[])
{

    if(argc < 1)
    {
        // this means we are manually generating the random number
    }
    else
    {
        std::string filePath(argv[1]);
    }

    std::srand(static_cast<unsigned int>(std::time(0)));
    // starting the thread for receiving the data
    std::thread t2(sendDataToFilterBlock);

    std::thread t1(randomNumberGen); // then start client

    t2.join();
    t1.join();

    return 0;
}