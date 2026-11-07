cmake_minimum_required(VERSION 4.2.2)
project(Compression_Algorithm_Comparison)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Compression_Algorithm_Comparison
        binaryHeader.cpp
        bitPacker.cpp
        Huffman.cpp
        LZW.cpp
        LZW.h
        main.cpp
        Huffman.h
        binaryHeader.h
        bitPacker.h
)
