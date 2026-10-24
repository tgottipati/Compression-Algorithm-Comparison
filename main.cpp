//
// Created by rsidd on 10/22/2026.
//
#include "LZW.h"
#include <cassert>
int main() {
    LZW lzw;
    std::string input = "test test test test test test";
    std::vector<int> encoded = lzw.encode(input);
    // for (auto i : encoded) {
    //     std::cout << i << std::endl;
    // }
    std::cout << "Encoded size: " << encoded.size() << std::endl;
    std::string decoded = lzw.decode(encoded);
    std::cout << decoded << std::endl;
    assert(decoded == input);
    return 0;
}