//
// Created by rsidd on 10/22/2026.
//

#ifndef COMPRESSION_ALGORITHM_COMPARISON_LZW_H
#define COMPRESSION_ALGORITHM_COMPARISON_LZW_H
#include <iostream>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>

class LZW {
public:
    LZW() = default;
    std::string decode(std::string input);
    std::vector<int> encode(const std::string& input);
    void resetDict();

private:
    std::unordered_map<std::string, int> dict;
    std::unordered_map<int, std::string> dictDecode;
};


#endif //COMPRESSION_ALGORITHM_COMPARISON_LZW_H
