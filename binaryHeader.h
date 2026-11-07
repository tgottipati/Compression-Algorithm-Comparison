//
// Created by tgott on 11/6/2026.
//

#ifndef COMPRESSION_ALGORITHM_COMPARISON_BINARYHEADER_H
#define COMPRESSION_ALGORITHM_COMPARISON_BINARYHEADER_H

#endif //COMPRESSION_ALGORITHM_COMPARISON_BINARYHEADER_H
#pragma once
#include <fstream>
#include <unordered_map>
#include <string>
#include <cstdint>

void writeHeader(std::ofstream& file,
                 const std::unordered_map<char, std::string>& huffmanMap,
                 uint64_t totalBits);

std::unordered_map<std::string, char> readHeader(std::ifstream& file,
                                                  uint64_t& totalBits);