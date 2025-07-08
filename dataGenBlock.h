#pragma once 
#include <iostream>
#include <stdio.h>


const char* internalPipeName = R"(\\.\pipe\MyPipe1)";

volatile uint8_t dataPts[2];