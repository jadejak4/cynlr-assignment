#pragma once 
#include <iostream>
#include <chrono>
#include <thread>      // For std::thread
#include <windows.h>   // For Windows APIs
#include <vector>
#include <fstream>
#include <sstream>



#define MAX UINT8_MAX
#define MIN 0


// this can be redefined as a template.
inline uint8_t generateRandomNumber()
{   
    return static_cast<uint8_t>(MIN + std::rand() % (MAX - MIN + 1));
}

std::vector<std::vector<uint8_t>> readCSV(const std::string& filename)
{
    std::vector<std::vector<uint8_t>> data;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        std::vector<uint8_t> row;
        std::stringstream ss(line);
        std::string value;

        while (std::getline(ss, value, ','))
        {
            row.push_back(static_cast<uint8_t>(std::stoi(value)));
        }

        data.push_back(row);
    }

    return data;
}