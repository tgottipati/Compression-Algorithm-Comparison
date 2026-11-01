//
// Created by rsidd on 10/22/2026.
//
#include "LZW.h"
#include <cassert>
#include <bits/locale_facets_nonio.h>

int main() {
    LZW lzw;
    std::string filename = "greatGastby";
    std::string inputFile = "../testFiles/inputFiles/" + filename + ".txt";

    std::string outputAddress = "../testFiles/outputFiles/";

    std::string lzwFile = outputAddress + filename + ".lzw";
    std::cout << lzwFile << std::endl;
    std::string outputFile = outputAddress + filename + "Remade.txt";
    lzw.compressFile(inputFile, lzwFile);
    lzw.decompressFile(lzwFile, outputFile);

}
