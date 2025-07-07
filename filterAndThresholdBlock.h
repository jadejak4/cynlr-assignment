#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <climits>
#include <chrono>
#include <thread>      // For std::thread
#include <windows.h>   // For Windows APIs

#define DEBUG_THRESHOLD_VALUE 10
#define WINDOW_SIZE 9



// this can be abstracted into a class
std::deque<uint8_t> windowElements;
bool windowFull = false; 
static constexpr double filterCofficients[WINDOW_SIZE] = {0.00025177, 0.008666992, 0.078025818, 0.24130249, 0.343757629, 0.24130249, 0.078025818, 0.008666992, 0.000125885};
double filteredOutput(uint8_t *val
    ues, int len);