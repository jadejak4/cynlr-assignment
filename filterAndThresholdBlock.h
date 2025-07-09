#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <climits>
#include <chrono>
#include <deque>
#include <thread>
#include <windows.h>
#include <atomic>
#include <vector>
#include <mutex>
#define DEBUG_THRESHOLD_VALUE 100.0
#define WINDOW_SIZE 9




// Global window buffer and status flag
std::atomic<bool> windowFull = false; 
std::vector<uint8_t> windowElements;
std::mutex windowMutex;


// Filter coefficients
static constexpr double filterCofficients[WINDOW_SIZE] = {
    0.00025177, 0.008666992, 0.078025818,
    0.24130249, 0.343757629, 0.24130249,
    0.078025818, 0.008666992, 0.000125885
    
};

// Function declarations
double filteredOutput(const std::vector<uint8_t> &values);
void filteringThread(double thresholdValue);
void slidingWindowThread();
