#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <chrono>
#include "huffman.h"
#include "binaryHeader.h"
#include "bitPacker.h"
#include "LZW.h"
#include <algorithm>
using namespace std;
unordered_map<char, string> Huffman(const vector<char>& chars);
vector<unsigned char> bitPacker(unordered_map<char, string> huffmanMap, vector<char> v, uint64_t& totalBits);
void writeHeader(ofstream& file, const unordered_map<char, string>& huffmanMap, uint64_t totalBits);
unordered_map<string, char> readHeader(ifstream& file, uint64_t& totalBits);

size_t getFileSize(const string& path) {
    ifstream f(path, ios::binary | ios::ate);
    return f.is_open() ? (size_t)f.tellg() : 0;
}



bool verifyCompressionIntegrity(const std::string& originalPath, const std::string& decompressedPath) {
    // Open in binary mode, seeking immediately to the end to get file size
    std::ifstream origFile(originalPath, std::ios::binary | std::ios::ate);
    std::ifstream decompFile(decompressedPath, std::ios::binary | std::ios::ate);

    if (!origFile.is_open() || !decompFile.is_open()) {
        std::cerr << "Validation Error: Could not open one or both files for verification.\n";
        return false;
    }

    // 1. Check File Size: If sizes differ, the algorithm failed
    std::streamsize origSize = origFile.tellg();
    std::streamsize decompSize = decompFile.tellg();

    if (origSize != decompSize) {
        std::cout << "FAIL: File sizes do not match!\n"
                  << "  Original size:     " << origSize << " bytes\n"
                  << "  Decompressed size: " << decompSize << " bytes\n";
        return false;
    }

    // Rewind back to the beginning to stream data
    origFile.seekg(0, std::ios::beg);
    decompFile.seekg(0, std::ios::beg);

    // 2. Stream and compare chunk by chunk (efficient for any file size)
    const size_t CHUNK_SIZE = 4096;
    std::vector<char> origBuffer(CHUNK_SIZE);
    std::vector<char> decompBuffer(CHUNK_SIZE);

    size_t byteOffset = 0;

    while (origFile && decompFile) {
        origFile.read(origBuffer.data(), CHUNK_SIZE);
        decompFile.read(decompBuffer.data(), CHUNK_SIZE);

        std::streamsize origBytesRead = origFile.gcount();
        std::streamsize decompBytesRead = decompFile.gcount();

        // Extra safeguard checking read lengths
        if (origBytesRead != decompBytesRead) {
            std::cout << "FAIL: Stream error or size mismatch during chunk read near byte " << byteOffset << "\n";
            return false;
        }

        // Byte-by-byte exact block comparison
        if (!std::equal(origBuffer.begin(), origBuffer.begin() + origBytesRead, decompBuffer.begin())) {
            // Find exactly where it broke to help you debug your algorithm
            for (std::streamsize i = 0; i < origBytesRead; ++i) {
                if (origBuffer[i] != decompBuffer[i]) {
                    std::cout << "FAIL: Mismatch found at byte offset: " << (byteOffset + i) << "\n";
                    return false;
                }
            }
        }

        byteOffset += origBytesRead;
    }

    std::cout << "SUCCESS: The decompressed file is an exact bitwise match of the original (" << byteOffset << " bytes verified).\n";
    return true;
}



int main() {


    string filename = "";

    cout << "Enter the file you would like to compress/decompress: \n";
    cin >> filename;

    string inputFile = "../testFiles/inputFiles/" + filename + ".txt";
    string outputDir = "../testFiles/outputFiles/";

    string huffCompressed  = outputDir + filename + ".bin";
    string huffDecompressed = outputDir + filename + "HuffRemade.txt";
    string lzwCompressed   = outputDir + filename + ".lzw";
    string lzwDecompressed = outputDir + filename + "LZWRemade.txt";

    // Read input once, share between both algorithms
    ifstream file(inputFile);
    if (!file) { cerr << "Unable to open input file\n"; return 1; }
    vector<char> v((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    size_t originalSize = getFileSize(inputFile);

    // ── Huffman ───────────────────────────────────────────────────────────────
    //cout << "Running Huffman...\n";

    auto huffCompStart = chrono::high_resolution_clock::now();
    unordered_map<char, string> huffmanMap = Huffman(v);
    uint64_t totalBits = 0;
    vector<unsigned char> bytes = bitPacker(huffmanMap, v, totalBits);
    ofstream huffOut(huffCompressed, ios::binary);
    if (!huffOut) { cerr << "Error opening Huffman output\n"; return 1; }
    writeHeader(huffOut, huffmanMap, totalBits);
    huffOut.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
    huffOut.close();
    auto huffCompEnd = chrono::high_resolution_clock::now();

    auto huffDecompStart = chrono::high_resolution_clock::now();
    ifstream huffIn(huffCompressed, ios::binary);
    uint64_t readTotalBits = 0;
    unordered_map<string, char> reverseMap = readHeader(huffIn, readTotalBits);
    vector<unsigned char> readBytes((istreambuf_iterator<char>(huffIn)), istreambuf_iterator<char>());
    huffIn.close();
    ofstream huffDecoded(huffDecompressed);
    string currentCode;
    uint64_t bitsRead = 0;
    for (unsigned char byte : readBytes) {
        for (int i = 7; i >= 0 && bitsRead < readTotalBits; i--, bitsRead++) {
            currentCode += ((byte >> i) & 1) ? '1' : '0';
            if (reverseMap.count(currentCode)) {
                huffDecoded << reverseMap[currentCode];
                currentCode.clear();
            }
        }
    }
    huffDecoded.close();
    auto huffDecompEnd = chrono::high_resolution_clock::now();

    // ── LZW ──────────────────────────────────────────────────────────────────
    //cout << "Running LZW...\n";
    LZW lzw;

    auto lzwCompStart = chrono::high_resolution_clock::now();
    lzw.compressFile(inputFile, lzwCompressed);
    auto lzwCompEnd = chrono::high_resolution_clock::now();

    auto lzwDecompStart = chrono::high_resolution_clock::now();
    lzw.decompressFile(lzwCompressed, lzwDecompressed);
    auto lzwDecompEnd = chrono::high_resolution_clock::now();

    // ── Report ────────────────────────────────────────────────────────────────
    size_t huffSize = getFileSize(huffCompressed);
    size_t lzwSize  = getFileSize(lzwCompressed);

    chrono::duration<double> huffCompTime   = huffCompEnd   - huffCompStart;
    chrono::duration<double> huffDecompTime = huffDecompEnd - huffDecompStart;
    chrono::duration<double> lzwCompTime    = lzwCompEnd    - lzwCompStart;
    chrono::duration<double> lzwDecompTime  = lzwDecompEnd  - lzwDecompStart;

    auto report = [&](const string& name, size_t compSize,
                      double compT, double decompT) {
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
    cout << "             HUFFMAN RESULTS            \n";
    cout << "========================================\n";
    report("Huffman", huffSize, huffCompTime.count(), huffDecompTime.count());

    cout << "\n========================================\n";
    cout << "              LZW RESULTS               \n";
    cout << "========================================\n";
    report("LZW", lzwSize, lzwCompTime.count(), lzwDecompTime.count());

    cout << "\n========================================\n";
    cout << "              WINNER                    \n";
    cout << "========================================\n";
    cout << " Best Compression : " << (huffSize < lzwSize ? "Huffman" : "LZW")
         << " (by " << (huffSize < lzwSize ? lzwSize - huffSize : huffSize - lzwSize)
         << " bytes)\n";
    cout << " Fastest Compress : "
         << (huffCompTime < lzwCompTime ? "Huffman" : "LZW") << "\n";
    cout << " Fastest Decomp   : "
         << (huffDecompTime < lzwDecompTime ? "Huffman" : "LZW") << "\n";
    cout << "========================================\n\n";

    cout << "Huffman Validation: " << endl;
    verifyCompressionIntegrity(inputFile, huffDecompressed);

    cout << "\nLZW Validation: " << endl;
    verifyCompressionIntegrity(inputFile, lzwDecompressed);
    return 0;
}

