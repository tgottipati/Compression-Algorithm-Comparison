#include <iostream>
//#include <filesystem>
#include "LZW.h"
#include <chrono>

int main() {
    LZW lzw;
    std::string filename = "war-and-peace";
    std::string inputFile = "../testFiles/inputFiles/" + filename + ".txt";
    std::string outputAddress = "../testFiles/outputFiles/";

    std::string lzwFile = outputAddress + filename + ".lzw";
    std::string outputFile = outputAddress + filename + "Remade.txt";

    //Compress File
    std::cout << "Starting compression pipeline...\n";
    auto compStartTime = std::chrono::high_resolution_clock::now();
    lzw.compressFile(inputFile, lzwFile);
    auto compEndTime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> compDuration = compEndTime - compStartTime;
    //Compression Analysis
    try {
            std::ifstream uncomp(inputFile, std::ios::binary | std::ios::ate);
            std::ifstream comp(lzwFile, std::ios::binary | std::ios::ate);
            if (uncomp.is_open() && comp.is_open()) {
                size_t originalSize = uncomp.tellg();
                size_t compressedSize = comp.tellg();

                //uintmax_t originalSize = std::filesystem::file_size(inputFile);
                //uintmax_t compressedSize = std::filesystem::file_size(lzwFile);

                if (originalSize > 0 && compressedSize > 0) {
                    double compressionFactor = static_cast<double>(originalSize) / compressedSize;
                    double savingsPercent = (1.0 - (static_cast<double>(compressedSize) / originalSize)) * 100.0;

                    std::cout << "\n========================================\n";
                    std::cout << "          COMPRESSION ANALYSIS          \n";
                    std::cout << "========================================\n";
                    std::cout << " Original Size      : " << originalSize << " bytes\n";
                    std::cout << " Compressed Size    : " << compressedSize << " bytes\n";
                    std::cout << " Compression Factor : " << compressionFactor << "x\n";
                    std::cout << " Space Savings      : " << savingsPercent << "%\n";
                    std::cout << "========================================\n\n";
                }
            }

    } catch (const std::exception& e) {
        std::cerr << "Filesystem error: " << e.what() << '\n';
    }

    //Decompress File
    auto decompStartTime = std::chrono::high_resolution_clock::now();
    lzw.decompressFile(lzwFile, outputFile);
    auto decompEndTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> decompDuration = decompEndTime - decompStartTime;

    //Report Timings
    std::cout << "Compression Time : " << compDuration.count() << " ms\n";
    std::cout << "Decompression Time : " << decompDuration.count() << " ms\n\n";

    return 0;
}