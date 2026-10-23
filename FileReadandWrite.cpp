//
// Created by tgott on 10/15/2026.
//
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;
unordered_map<char, string> Huffman(const vector<char> chars);

std::vector<unsigned char> bitPacker(
    std::unordered_map<char, std::string> huffmanMap,
    std::vector<char> input
);


// int main() {
//     ifstream file("greatGatsby.txt"); // open file for reading
//     if (!file) {
//         cout << "Unable to open file" << endl;
//         return 1;
//     }
//
//     char ch;
//     vector<char> v;
//     while (file.get(ch)) { // read one char at a time
//         v.push_back(ch);
//     }
//     file.close();
//     unordered_map<char,string> huffmanMap=Huffman(v);
//     vector<unsigned char> bytes = bitPacker(huffmanMap, v);;
//     ofstream outputfile("compressedFile.bin", ios::binary | ios::out);
//     if (outputfile.is_open()) {
//         outputfile.close();
//         cout << "Data written to file successfully." << endl;
//     } else {
//         cerr << "Error opening file for writing." << endl;
//     }
//     return 0;
// }
