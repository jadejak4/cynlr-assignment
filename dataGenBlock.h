#pragma once 
#include <iostream>
#include <stdio.h>
#include <atomic>
#include <vector>


std::atomic<int> dataPt[2];
std::atomic<bool> dataUpdated = false;