//
// Created by rsidd on 10/22/2026.
//
#include "LZW.h"
#include <cassert>
#include <bits/locale_facets_nonio.h>

int main() {
    LZW lzw;

    std::string input = "TOBEORNOTTOBEORTOBEORNOT";

    // Step 1: Encode the original string with LZW.
    std::vector<int> codes = lzw.encode(input);

    std::cout << "Original string: " << input << '\n';

    std::cout << "Encoded codes: ";
    for (int code : codes) {
        std::cout << code << " ";
    }
    std::cout << '\n';

    // Step 2: Pack the codes into bytes.
    std::vector<unsigned char> packed = lzw.bitPacker(codes);

    std::cout << "Packed bytes (decimal): ";
    for (unsigned char byte : packed) {
        std::cout << static_cast<int>(byte) << " ";
    }
    std::cout << '\n';


    std::cout << "Original size: " << input.size()
              << " bytes\n";
    std::cout << "Packed size: " << packed.size()
              << " bytes\n";
}
