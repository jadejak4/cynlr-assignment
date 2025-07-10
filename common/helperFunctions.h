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


void launchProcess(const std::string& processPath, const std::string& arguments = "") {
    std::string command = processPath;
    if (!arguments.empty()) {
        command += " " + arguments;
    }

    // Create a mutable buffer for CreateProcessA
    std::vector<char> cmdBuffer(command.begin(), command.end());
    cmdBuffer.push_back('\0');  // Null-terminate

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    BOOL result = CreateProcessA(
        nullptr,
        cmdBuffer.data(),  // ✅ Mutable null-terminated C-string
        nullptr, nullptr, FALSE, 0,
        nullptr, nullptr,
        &si, &pi
    );

    if (result) {
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    } else {
        std::cerr << "Failed to launch: " << command << "\n";
    }
}