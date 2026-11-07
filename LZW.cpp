//
// Created by rsidd on 10/22/2026.
//

#include "LZW.h"

void LZW::resetDict() {
    dict.clear();
    for (int i = 0; i < 256; i++) {
        std::string ch(1, static_cast<char>(i));
        dict[ch] = i;
    }
}
void LZW::resetDictDecode() {
    dictDecode.clear();
    for (int i = 0; i < 256; i++) {
        std::string ch(1, static_cast<char>(i));
        dictDecode[i] = ch;
    }
}
std::vector<int> LZW::encode(const std::string& input) {
    resetDict();
    std::vector<int> codeWord;
    if (input.empty()) {
        return codeWord;
    }
    std::string S = input.substr(0, 1);
    int code = 256;
    for (char& C : input.substr(1)) {
        if (dict.find(S+C) != dict.end()) {
            S+=C;
        }else {
            codeWord.push_back(dict.at(S));
            dict[S+C] = code++;
            S = C;
        }
    }
    codeWord.push_back(dict.at(S));
    return codeWord;
}

std::string LZW::decode(const std::vector<int>&  codeWords) {
    resetDictDecode();
    int old = codeWords[0];
    std::string decoded;
    std::string S = dictDecode[old];
    std::string C;
    C+=S[0];
    int nextCode = 256;
    decoded.append(dictDecode[old]);
    for (int i = 1; i < codeWords.size(); i++) {
        int curr = codeWords[i];
        if (dictDecode.find(curr) == dictDecode.end()) {
            S = dictDecode[old];
            S.append(C);
        }else {
            S = dictDecode[curr];
        }
        decoded.append(S);
        C = S[0];
        dictDecode[nextCode++] = dictDecode[old]+C;
        old = curr;
    }

    return decoded;
}

std::vector<unsigned char> LZW::bitPacker(const std::vector<int>& codeWords) {
    int bitCount = 0;
    unsigned char byte = 0;
    std::vector<unsigned char> bytes;
    int nextCode = 256;
    for (int code: codeWords) {
        int bitsPerInt = 9;
        int threshold = 512;
        while (nextCode >= threshold && bitsPerInt < maxBitsPerInt) {
            bitsPerInt++;
            threshold <<=1;
        }
        for (int i = bitsPerInt-1; i >= 0; i-- ) {
            const unsigned char bit = (code>>i) & 1;
            byte = byte*2+bit;
            ++bitCount;
            if (bitCount == 8) {
                bytes.push_back(byte);
                byte = 0;
                bitCount = 0;
            }
        }
        ++nextCode;
    }
    if (bitCount > 0) {
        byte <<= (8 - bitCount);
        bytes.push_back(byte);
    }
    return bytes;
}

std::vector<int> LZW::bitUnpacker(const std::vector<unsigned char>& bytes, size_t totalCodes) {
    std::vector<int> codeWords;
    codeWords.reserve(totalCodes);

    size_t byteIndex = 0;
    unsigned int bitBuffer = 0;
    int bitCount = 0;
    int nextCode = 256;
    for (size_t i = 0; i<totalCodes; i++) {
        int bitsPerInt = 9;
        int threshold = 512;
        while (nextCode >= threshold && bitsPerInt < maxBitsPerInt) {
            bitsPerInt++;
            threshold <<=1;
        }

        while (bitCount < bitsPerInt) {
            if (byteIndex < bytes.size()) {
                bitBuffer = (bitBuffer <<8) | bytes[byteIndex++];
                bitCount+=8;
            }else {
                break;
            }
        }
        int shift = bitCount -bitsPerInt;
        int code = (bitBuffer >> shift) & ((1<<bitsPerInt)-1);
        bitCount -= bitsPerInt;
        bitBuffer &= (1<<bitCount)-1;

        codeWords.push_back(code);
        ++nextCode;
    }
    return codeWords;
}

void LZW::compressFile(const std::string &inputFile, const std::string &outputFile) {
    std::ifstream textFile(inputFile, std::ios::binary);
    if (!textFile) {
            std::cout << "Unable to open uncompressed text file" << std::endl;
            return;
    }
    std::stringstream ss;
    ss<< textFile.rdbuf();
    std::string input = ss.str();
    textFile.close();

    std::vector<int> codeWords = encode(input);
    std::vector<unsigned char> bytes = bitPacker(codeWords);
    size_t totalCodes = codeWords.size();

    std::ofstream lzwFile(outputFile, std::ios::binary);
    if (!lzwFile) {
        std::cout << "Unable to write lzw file" << std::endl;
        return;
    }
    lzwFile.write(reinterpret_cast<char*>(&totalCodes), sizeof(totalCodes));
    lzwFile.write(reinterpret_cast<char*>(bytes.data()), bytes.size());
    lzwFile.close();

    //std::cout << "Successfully compressed " << inputFile << " -> " << outputFile << "\n";
}

void LZW::decompressFile(const std::string &inputFile, const std::string &outputFile) {
    std::ifstream lzwFile(inputFile, std::ios::binary);
    if (!lzwFile) {
        std::cout << "Unable to open lzw file" << std::endl;
        return;
    }
    size_t totalCodes = 0;
    lzwFile.read(reinterpret_cast<char*>(&totalCodes), sizeof(totalCodes));
    std::vector<unsigned char> bytes(
    (std::istreambuf_iterator<char>(lzwFile)),
    std::istreambuf_iterator<char>()
    );

    std::vector<int> codeWords = bitUnpacker(bytes, totalCodes);
    std::string uncompressedText = decode(codeWords);
    std::ofstream textFile(outputFile, std::ios::binary);
    if (!textFile) {
        std::cout << "Unable to write decompressed text file" << std::endl;
        return;
    }
    textFile << uncompressedText;
    textFile.close();
    //std::cout << "Successfully decompressed " << inputFile << " -> " << outputFile << "\n";
}