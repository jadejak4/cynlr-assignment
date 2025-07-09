#pragma once 
#include <iostream>
#include <stdio.h>
#include <atomic>

const char* internalPipeName = R"(\\.\pipe\MyPipe1)";

volatile std::atomic<int> dataPt[2];