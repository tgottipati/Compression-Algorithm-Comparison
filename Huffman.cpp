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
using namespace std;
//return unordered map of char-freq key value pairs


struct Node {
    char character;
    int frequency;
    Node* left;
    Node* right;
    Node(char c, int f) {
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
vector<Node*> countFrequencies(vector<char> chars) {
    unordered_map<char,int> freq;
    vector<Node*> leafNodes;
    for (char c : chars) {
        freq[c]++;
    }
    for (auto c : freq) {
        leafNodes.push_back(new Node(c.first, c.second));
    }
}
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->frequency > b->frequency;
    }
};
void Traversal(Node* root) {}
void Huffman(vector<char> chars) {
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
}