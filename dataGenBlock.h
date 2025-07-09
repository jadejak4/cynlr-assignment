#pragma once 
#include <iostream>
#include <stdio.h>
#include <atomic>


volatile std::atomic<int> dataPt[2];
std::atomic<bool> dataUpdated = false;