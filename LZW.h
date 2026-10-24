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
#include <algorithm>
#include <cmath>

class LZW {
public:
    LZW() = default;
    std::string decode(const std::vector<int>& codeWords);
    std::vector<int> encode(const std::string& input);
    std::vector<unsigned char> bitPacker(const std::vector<int>& codeWords);

private:
    std::unordered_map<std::string, int> dict;
    std::unordered_map<int, std::string> dictDecode;
    void resetDict();
    void resetDictDecode();
};


#endif //COMPRESSION_ALGORITHM_COMPARISON_LZW_H
