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

unordered_map<string, char> readHeader(ifstream& file,uint64_t& totalBits) {
    uint16_t mapSize;

    file.read(reinterpret_cast<char*>(&mapSize),sizeof(mapSize));

    file.read(reinterpret_cast<char*>(&totalBits),sizeof(totalBits));

    unordered_map<string, char> reverseMap;

    for (uint16_t i = 0; i < mapSize; i++) {
        char character;
        uint16_t codeLength;

        file.read(&character, sizeof(character));

        file.read(reinterpret_cast<char*>(&codeLength),
                  sizeof(codeLength));

        string code(codeLength, '\0');
        file.read(&code[0], codeLength);

        reverseMap[code] = character;
    }

    return reverseMap;
}

