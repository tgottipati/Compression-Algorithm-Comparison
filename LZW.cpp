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
