//
// Created by tgott on 10/15/2026.
//
#include <fstream>
#include <iostream>
#include <vector>
#include "Huffman.cpp"
using namespace std;

int main() {
    ifstream file("example.txt"); // open file for reading
    if (!file) {
        cout << "Unable to open file" << endl;
        return 1;
    }

    char ch;
    vector<char> v;
    while (file.get(ch)) { // read one char at a time
        v.push_back(ch);
    }
    file.close();
    unordered_map<char,string> huffmanMap=Huffman(v);
    ofstream outputfile("compressedFile.bin", ios::binary | ios::out);
    if (outputfile.is_open()) {
        for (char letter : v) {
            outputfile.write(huffmanMap[letter].c_str(), huffmanMap[letter].size());
        }
        outputfile.close();
        cout << "Data written to file successfully." << endl;
    } else {
        cerr << "Error opening file for writing." << endl;
    }
    return 0;
}
