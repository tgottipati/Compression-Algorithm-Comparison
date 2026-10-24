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
