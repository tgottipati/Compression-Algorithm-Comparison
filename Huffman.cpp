//
// Created by tgott on 10/11/2026.
//
//find the frequency of each character in text
//place each frequency and values in priority queue (min heap)
//create tree
//new node: left:min right:2nd min value:frequency min+min2
//finish queue and assign left bracnh 0 and right branch 1 each thing has unique code to translate
#include <vector>
#include <unordered_map>
#include <queue>
#include <string>
#include <iostream>
using namespace std;
//return unordered map of char-freq key value pairs


struct Node {
    char character;
    unsigned int frequency;
    Node* left;
    Node* right;
    Node(char c, unsigned int f) {
        character = c;
        frequency = f;
        left = nullptr;
        right = nullptr;
    }
    Node(Node* l, Node* r) {
        character = '\0';  // Null character
        frequency = l->frequency + r->frequency;
        left = l;
        right = r;
    }
};
vector<Node*> countFrequencies(const vector<char> chars) {
    unordered_map<char,unsigned int> freq;
    vector<Node*> leafNodes;
    for (char c : chars) {
        freq[c]++;
    }
    for (auto c : freq) {
        leafNodes.push_back(new Node(c.first, c.second));
    }
    return leafNodes;
}
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->frequency > b->frequency;
    }
};
void Traversal(const Node* root,string code,unordered_map<char,string>& codes) {
    if (root == nullptr) {
        return;
    }
    // If this is a leaf, save its code, if only 1 unique character give it code 0
    if (root->left == nullptr && root->right == nullptr) {
        codes[root->character] = code.empty() ? "0" : code;
        return;
    }

    Traversal(root->left, code + "0", codes);
    Traversal(root->right, code + "1", codes);
}
void deleteTree(Node* root) {
    if (root == nullptr) {
        return;
    }

    deleteTree(root->left);
    deleteTree(root->right);

    delete root;
}
unordered_map<char,string> Huffman(const vector<char> chars) {
    vector<Node*> leafNodes =countFrequencies(chars);
    priority_queue<Node*, vector<Node*>, Compare> pq;
    //add all nodes to priority Queue
    for(int i = 0; i < leafNodes.size(); i++) {
        pq.push(leafNodes[i]);
    }
    //add sorted nodes in queue to tree
    while (pq.size() > 1) {
        Node* left = pq.top();
        pq.pop();

        Node* right = pq.top();
        pq.pop();

        Node* parent = new Node(left, right);
        pq.push(parent);
    }
    if (pq.empty()) {
        return {};
    }
    Node* root = pq.top();
    unordered_map<char, string> codes;
    Traversal(root, "", codes);
    for (auto& pair : codes) {
        cout << pair.first << ": " << pair.second << '\n';
    }
    deleteTree(root);
    return codes;
}