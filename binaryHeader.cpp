//
// Created by tgott on 10/15/2026.
//

#include <fstream>
#include <string>
#include <unordered_map>
#include <cstdint>
using namespace std;

void writeHeader(ofstream& file, const unordered_map<char, string>& huffmanMap,uint64_t totalBits) {
    // Write number of characters in the Huffman map
    uint16_t mapSize = huffmanMap.size();
    file.write(
        reinterpret_cast<const char*>(&mapSize),
        sizeof(mapSize)
    );
    // Write total number of valid encoded bits
    file.write(
        reinterpret_cast<const char*>(&totalBits),
        sizeof(totalBits)
    );

    // Write each character and its Huffman code
    for (const auto& entry : huffmanMap) {
        char character = entry.first;
        const string& code = entry.second;

        uint16_t codeLength = code.size();

        // Write the character
        file.write(&character, sizeof(character));

        // Write the length of its code
        file.write(
            reinterpret_cast<const char*>(&codeLength),
            sizeof(codeLength)
        );

        // Write the code as ASCII '0' and '1'
        file.write(code.data(), code.size());
    }
}
