cmake_minimum_required(VERSION 4.3)
project(Compression_Algorithm_Comparison)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Compression_Algorithm_Comparison
        binaryHeader.cpp
        bitPacker.cpp
        FileReadandWrite.cpp
        Huffman.cpp
        LZW.cpp
        LZW.h
        main.cpp
)
