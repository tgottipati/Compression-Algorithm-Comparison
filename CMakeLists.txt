cmake_minimum_required(VERSION 4.2)
project(Compression_Algorithm_Comparison)

set(CMAKE_CXX_STANDARD 14)

add_executable(Compression_Algorithm_Comparison
        Huffman.cpp
        FileReadandWrite.cpp
        bitPacker.cpp
        binaryHeader.cpp)
