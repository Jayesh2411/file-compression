#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>

using namespace std;

// Node structure for the Huffman Tree
struct Node {
    char ch;
    int freq;
    Node *left, *right;

    Node(char c, int f, Node* l = nullptr, Node* r = nullptr) {
        ch = c;
        freq = f;
        left = l;
        right = r;
    }
};

// Comparison to ensure the smallest frequency is at the top (Min-Heap)
struct Compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

// Generates 0/1 codes by traversing the tree
void encode(Node* root, string str, unordered_map<char, string> &huffmanCode) {
    if (root == nullptr) return;

    // If it's a leaf node, it contains a character
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = (str == "") ? "0" : str; 
    }

    encode(root->left, str + "0", huffmanCode);
    encode(root->right, str + "1", huffmanCode);
}

// Decodes the binary string back to text
string decode(Node* root, string s) {
    if (root == nullptr) return "";
    
    string ans = "";
    Node* curr = root;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '0')
            curr = curr->left;
        else
            curr = curr->right;

        // If leaf node is reached
        if (!curr->left && !curr->right) {
            ans += curr->ch;
            curr = root;
        }
    }
    return ans;
}

void buildHuffmanTree(string text) {
    if (text.empty()) {
        cout << "String is empty!" << endl;
        return;
    }

    unordered_map<char, int> freq;
    for (char ch : text) freq[ch]++;

    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto pair : freq) {
        pq.push(new Node(pair.first, pair.second));
    }

    // --- SINGLE CHARACTER FIX ---
    // If only one unique character exists, we manually create a parent
    if (pq.size() == 1) {
        Node* left = pq.top(); pq.pop();
        pq.push(new Node('\0', left->freq, left, nullptr));
    }

    // Standard Huffman Tree Building
    while (pq.size() > 1) {
        Node *left = pq.top(); pq.pop();
        Node *right = pq.top(); pq.pop();
        int sum = left->freq + right->freq;
        pq.push(new Node('\0', sum, left, right));
    }

    Node* root = pq.top();

    // Map to store character codes
    unordered_map<char, string> huffmanCode;
    encode(root, "", huffmanCode);

    // Build the encoded binary string
    string encodedStr = "";
    for (char ch : text) encodedStr += huffmanCode[ch];

    // Decode it back to verify
    string decodedStr = decode(root, encodedStr);

    cout << "Original: " << text << endl;
    cout << "Encoded:  " << encodedStr << endl;
    cout << "Decoded:  " << decodedStr << endl;

    // Statistics
    double originalBits = text.length() * 8.0;
    double compressedBits = encodedStr.length();
    double ratio = (compressedBits / originalBits) * 100.0;

    cout << "\nSpace Saved: " << 100.0 - ratio << "%" << endl;
}

int main() {
    string text;
    cout << "Enter text to compress: ";
    getline(cin, text);
    buildHuffmanTree(text);
    return 0;
}
