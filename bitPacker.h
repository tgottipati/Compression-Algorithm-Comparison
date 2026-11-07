//
// Created by tgott on 11/6/2026.
//

#ifndef COMPRESSION_ALGORITHM_COMPARISON_BITPACKER_H
#define COMPRESSION_ALGORITHM_COMPARISON_BITPACKER_H

#endif //COMPRESSION_ALGORITHM_COMPARISON_BITPACKER_H
#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <cstdint>

std::vector<unsigned char> bitPacker(
    std::unordered_map<char, std::string> huffmanMap,
    std::vector<char> v,
    uint64_t& totalBits
);