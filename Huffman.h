//
// Created by tgott on 11/6/2026.
//

#ifndef COMPRESSION_ALGORITHM_COMPARISON_HUFFMAN_H
#define COMPRESSION_ALGORITHM_COMPARISON_HUFFMAN_H

#endif //COMPRESSION_ALGORITHM_COMPARISON_HUFFMAN_H

#pragma once
#include <vector>
#include <unordered_map>
#include <string>

std::unordered_map<char, std::string> Huffman(const std::vector<char>& chars);
struct Node {
    char character;
    unsigned int frequency;
    Node* left;
    Node* right;
    Node(char c, unsigned int f);
    Node(Node* l, Node* r);
};

void deleteTree(Node* root);