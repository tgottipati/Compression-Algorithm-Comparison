#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <chrono>
#include <cstdint>
#include "Huffman.h"
#include "binaryHeader.h"
#include "bitPacker.h"
#include "LZW.h"

using namespace std;

size_t getFileSize(const string& path) {
    ifstream f(path, ios::binary | ios::ate);
    return f.is_open() ? (size_t)f.tellg() : 0;
}

int main() {
    string filename = "war-and-peace";
    string inputFile = "testFiles/inputFiles/" + filename + ".txt";
    string outputDir = "testFiles/outputFiles/";

    string huffCompressed   = outputDir + filename + ".bin";
    string huffDecompressed = outputDir + filename + "HuffRemade.txt";
    string lzwCompressed    = outputDir + filename + ".lzw";
    string lzwDecompressed  = outputDir + filename + "LZWRemade.txt";

    // Read input file once
    ifstream file(inputFile);
    if (!file) { cerr << "Unable to open input file: " << inputFile << "\n"; return 1; }
    vector<char> v((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    size_t originalSize = getFileSize(inputFile);

    // ── Huffman Compress ─────────────────────────────────────────────────────
    cout << "Running Huffman compression...\n";
    auto huffCompStart = chrono::high_resolution_clock::now();

    unordered_map<char, string> huffmanMap = Huffman(v);
    uint64_t totalBits = 0;
    vector<unsigned char> bytes = bitPacker(huffmanMap, v, totalBits);

    ofstream huffOut(huffCompressed, ios::binary);
    if (!huffOut) { cerr << "Error opening Huffman output file\n"; return 1; }
    writeHeader(huffOut, huffmanMap, totalBits);
    huffOut.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
    huffOut.close();

    auto huffCompEnd = chrono::high_resolution_clock::now();

    // ── Huffman Decompress ───────────────────────────────────────────────────
    cout << "Running Huffman decompression...\n";
    auto huffDecompStart = chrono::high_resolution_clock::now();

    ifstream huffIn(huffCompressed, ios::binary);
    if (!huffIn) { cerr << "Error opening compressed Huffman file\n"; return 1; }
    uint64_t readTotalBits = 0;
    unordered_map<string, char> reverseMap = readHeader(huffIn, readTotalBits);
    vector<unsigned char> readBytes((istreambuf_iterator<char>(huffIn)), istreambuf_iterator<char>());
    huffIn.close();

    // Reconstruct tree from reverseMap
    Node* root = new Node('\0', 0);
    for (const auto& entry : reverseMap) {
        const string& code = entry.first;
        char character = entry.second;
        Node* current = root;
        for (char bit : code) {
            if (bit == '0') {
                if (!current->left)  current->left  = new Node('\0', 0);
                current = current->left;
            } else {
                if (!current->right) current->right = new Node('\0', 0);
                current = current->right;
            }
        }
        current->character = character;
    }

    // Decode using tree traversal
    Node* current = root;
    ofstream huffDecoded(huffDecompressed);
    if (!huffDecoded) { cerr << "Error opening Huffman decompressed output\n"; return 1; }
    uint64_t bitsRead = 0;
    for (unsigned char byte : readBytes) {
        for (int i = 7; i >= 0 && bitsRead < readTotalBits; i--, bitsRead++) {
            current = ((byte >> i) & 1) ? current->right : current->left;
            if (current->left == nullptr && current->right == nullptr) {
                huffDecoded << current->character;
                current = root;
            }
        }
    }
    huffDecoded.close();
    deleteTree(root);

    auto huffDecompEnd = chrono::high_resolution_clock::now();

    // ── LZW Compress ─────────────────────────────────────────────────────────
    cout << "Running LZW compression...\n";
    LZW lzw;
    auto lzwCompStart = chrono::high_resolution_clock::now();
    lzw.compressFile(inputFile, lzwCompressed);
    auto lzwCompEnd = chrono::high_resolution_clock::now();

    // ── LZW Decompress ───────────────────────────────────────────────────────
    cout << "Running LZW decompression...\n";
    auto lzwDecompStart = chrono::high_resolution_clock::now();
    lzw.decompressFile(lzwCompressed, lzwDecompressed);
    auto lzwDecompEnd = chrono::high_resolution_clock::now();

    // ── Sizes + Timings ───────────────────────────────────────────────────────
    size_t huffSize = getFileSize(huffCompressed);
    size_t lzwSize  = getFileSize(lzwCompressed);

    chrono::duration<double> huffCompTime   = huffCompEnd   - huffCompStart;
    chrono::duration<double> huffDecompTime = huffDecompEnd - huffDecompStart;
    chrono::duration<double> lzwCompTime    = lzwCompEnd    - lzwCompStart;
    chrono::duration<double> lzwDecompTime  = lzwDecompEnd  - lzwDecompStart;

    // ── Report ────────────────────────────────────────────────────────────────
    auto printReport = [&](const string& name, size_t compSize, double compT, double decompT) {
        double factor  = static_cast<double>(originalSize) / compSize;
        double savings = (1.0 - static_cast<double>(compSize) / originalSize) * 100.0;
        cout << " Original Size      : " << originalSize << " bytes\n";
        cout << " Compressed Size    : " << compSize     << " bytes\n";
        cout << " Compression Factor : " << factor       << "x\n";
        cout << " Space Savings      : " << savings      << "%\n";
        cout << " Compression Time   : " << compT        << "s\n";
        cout << " Decompression Time : " << decompT      << "s\n";
    };

    cout << "\n========================================\n";
    cout << "           HUFFMAN RESULTS              \n";
    cout << "========================================\n";
    printReport("Huffman", huffSize, huffCompTime.count(), huffDecompTime.count());

    cout << "\n========================================\n";
    cout << "             LZW RESULTS                \n";
    cout << "========================================\n";
    printReport("LZW", lzwSize, lzwCompTime.count(), lzwDecompTime.count());

    cout << "\n========================================\n";
    cout << "               WINNER                   \n";
    cout << "========================================\n";
    cout << " Best Compression : " << (huffSize < lzwSize ? "Huffman" : "LZW")
         << " (by " << (huffSize < lzwSize ? lzwSize - huffSize : huffSize - lzwSize)
         << " bytes)\n";
    cout << " Fastest Compress : " << (huffCompTime   < lzwCompTime   ? "Huffman" : "LZW") << "\n";
    cout << " Fastest Decomp   : " << (huffDecompTime < lzwDecompTime ? "Huffman" : "LZW") << "\n";
    cout << "========================================\n\n";

    return 0;
}