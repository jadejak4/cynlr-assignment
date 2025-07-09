#pragma once 
#include <iostream>
#include <chrono>
#include <thread>      // For std::thread
#include <windows.h>   // For Windows APIs



#define MAX UINT8_MAX
#define MIN 0


// this can be redefined as a template.
inline uint8_t generateRandomNumber()
{   
    return static_cast<uint8_t>(MIN + std::rand() % (MAX - MIN + 1));
}