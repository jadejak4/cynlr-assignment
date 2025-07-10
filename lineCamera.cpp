#include <windows.h>
#include <iostream>
#include "common/helperFunctions.h"



int main(int argc, char* argv[]) {
    if (argc == 2) {
        launchProcess("bin\\dataGenBlock.exe");
        launchProcess("bin\\filterAndThresholdBlock.exe", argv[1]);
    } else if (argc == 4) {
        std::string combinedArgs = std::string(argv[2]) + " " + argv[3];
        launchProcess("bin\\dataGenBlock.exe", combinedArgs);
        launchProcess("bin\\filterAndThresholdBlock.exe", argv[1]);
    }
    else
    {
        std::cerr << "Invalid number of args " << std::endl;
        return 1;
    }

    return 0;
}