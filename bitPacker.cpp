//
// Created by tgott on 10/15/2026.
//
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
vector <unsigned char> bitPacker(unordered_map<char,string> huffmanMap, vector<char> v) {
    int bitCount=0;
    unsigned char byte = 0;
    vector <unsigned char> bytes;
    for (char letter : v) {
        string code=huffmanMap[letter];
        for (char bit : code) {
            byte = (byte << 1) | (bit == '1' ? 1 : 0);
            bitCount++;
            if (bitCount == 8) {
                bytes.push_back(byte);
                byte = 0;
                bitCount = 0;
            }
        }
    }
    if (bitCount > 0) {
        byte <<= (8 - bitCount);
        bytes.push_back(byte);
    }
    return bytes;

}