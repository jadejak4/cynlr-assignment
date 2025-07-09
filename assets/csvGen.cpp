#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstdint>

#define MIN 0
#define MAX 255
#define DEFAULT_NUM_OF_ROWS 8

inline uint8_t generateRandomNumber()
{
    return static_cast<uint8_t>(MIN + std::rand() % (MAX - MIN + 1));
}

void generateAndSaveCSV(int m, const std::string& filename = "output.csv")
{
    const int rows = 1000;

    std::ofstream outFile(filename);
    if (!outFile.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    std::srand(static_cast<unsigned int>(std::time(nullptr)));  // Seed RNG
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            outFile << static_cast<int>(generateRandomNumber());
            if (j < m - 1)
                outFile << ",";
        }
        outFile << "\n";
    }
    outFile.close();
    std::cout << "CSV file generated: " << filename << std::endl;
}

int main(int argc, char* argv[])
{
    int m;
    if(argc < 2)
    {
        // no input number of rows has been provided, take default
        m = DEFAULT_NUM_OF_ROWS;
    }
    else
    {
        m = std::stoi(argv[1]);
    }
    generateAndSaveCSV(m);
}