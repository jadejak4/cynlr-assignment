#include "filterAndThresholdBlock.h"

double filteredOutput(const uint8_t *values, int len)
{
    double multipliedVal = 0;
    if(len != WINDOW_SIZE)
        return double(0);
    for(int i = 0; i < WINDOW_SIZE; i++)
    {
        multipliedVal += double(values[i]) * filterCofficients[i];
        std::cout << multipliedVal << std::endl; 
    }

    return multipliedVal;
}    

void slidingWindowThread(int thresholdValue)
{
    std::vector<int> windowElements(WINDOW_SIZE);


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