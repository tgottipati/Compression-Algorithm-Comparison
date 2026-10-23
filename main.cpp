//
// Created by rsidd on 10/22/2026.
//
#include "LZW.h"

int main() {
    LZW lzw;
    std::string input = "WYS*WYGWYS*WYSWYSG";
    std::vector<int> encoded = lzw.encode(input);
    for (auto i : encoded) {
        std::cout << i << std::endl;
    }

    return 0;
}