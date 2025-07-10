#include "common/commonArgs.h"
#include "common/sharedMemory.h"
#include "common/helperFunctions.h"
#include "dataGenBlock.h"

void randomNumberGen()
{
    while (1)
    {
        if(!dataUpdated)
        {
            dataPt[0] = generateRandomNumber();
            dataPt[1] = generateRandomNumber();
            dataUpdated = true;
        }
        
    }
}

void csvDataFeeder(const std::string& filename, int m)
{
    std::vector<std::vector<uint8_t>> data = readCSV(filename);
    for(int i = 0; i < data.size )

    if (data.empty())
    {
        std::cerr << "CSV is empty or failed to load.\n";
        return;
    }

    size_t row = 0;
    size_t col = 0;

    while (true)
    {
        if (!dataUpdated)
        {
            if (row >= data.size())
            {
                row = 0;  
                col = 0;
            }

            if (col + 1 >= m)
            {
                // Not enough data left in this row — move to next
                ++row;
                col = 0;
                continue;
            }

            {
                dataPt[0] = data[row][col];
                dataPt[1] = data[row][col + 1];
                dataUpdated = true;
            }

            col += 2;
        }
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
        dataUpdated = false;
        writeToSharedMemory(ptr, localBuffer, sizeof(localBuffer));
    }

    unmapSharedMemory(ptr);
    closeSharedMemory(hMap);
}


int main(int argc, char *argv[])
{
    std::srand(static_cast<unsigned int>(std::time(0)));
    std::thread t1;
    // this is testing mode. Meaning we have to generate the 
    if(argc == 1)
    {
        t1 = std::thread(randomNumberGen);
    }
    else if(argc == 3)
    {
        std::string filePath(argv[1]);
        // number of cols
        int m = std::stoi(argv[2]);
        t1 = std::thread(csvDataFeeder, filePath, m); 
    }
    else
    {
        std::cerr << "Invalid number of args " << std::endl;
        return 1;
    }


    std::thread t2(sendDataToFilterBlock);

    
    t2.join();
    t1.join();

    return 0;
}