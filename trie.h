#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>
#include <stdexcept>
#include <bitset>
#include <iostream>

#include "reader.h"
#include "huffman.h"

#include <memory>
#include <vector>
#include <unordered_map>


class Trie {
public:
    struct TrieNode {
        std::array<size_t, 2> childs = { 0, 0 };
        bool end_of_code = false;
        char value;
    };

    void Insert(int length, uint32_t code, char val);

    char Find(BitReader* b);

    size_t Size();

private:
    std::vector<TrieNode> nodes_{1};
};


class HuffmanTable {
public:
    void SetBitReader(BitReader* b);

    void BuildTable(const std::vector<char>& freqs, const std::vector<char>& data);

    char DecodeSymbol();

    size_t Size();

private:
    Trie t_;
    BitReader* b_;
};



void Trie::Insert(int length, uint32_t code, char val) {
    size_t curr_node_index = 0;
    for (int i = 0; i < length; ++i) {
        if (curr_node_index >= nodes_.size())
            throw std::logic_error("error while building DHT");
        unsigned int direction = std::bitset<32>{code}[length - 1 - i];
        if (!nodes_[curr_node_index].childs[direction]) {
            nodes_[curr_node_index].childs[direction] = nodes_.size();
            nodes_.emplace_back();
        }
        curr_node_index = nodes_[curr_node_index].childs[direction];
    }
    nodes_[curr_node_index].end_of_code = true;
    nodes_[curr_node_index].value = val;
}

char Trie::Find(BitReader* b) {
    size_t curr_node_index = 0;
    if (nodes_.empty())
        throw std::logic_error("empty DHT");
    while (curr_node_index < nodes_.size()) {
        if (nodes_[curr_node_index].end_of_code) break;
        try {
            int direction = b->GetCurrBit();
            curr_node_index = nodes_[curr_node_index].childs[direction];
            b->IncrementPtr();
        } catch (...) {
            return 0;
        }
    }
    if (curr_node_index >= nodes_.size())
        return 0;
    return nodes_[curr_node_index].value;
}

size_t Trie::Size() {
    return nodes_.size();
}


void HuffmanTable::SetBitReader(BitReader* b) {
    b_ = b;
}

void HuffmanTable::BuildTable(const std::vector<char>& freqs, const std::vector<char>& data) {
    size_t index = 0;
    uint32_t code = 0;
    for (auto i = 0; i < 16; ++i) {
        for (auto j = 0; j < static_cast<int>(freqs[i]); ++j) {
            if (index >= data.size()) break;
            t_.Insert(i + 1, code, data[index++]);
            ++code;
        }
        code <<= 1;
    }
}

char HuffmanTable::DecodeSymbol() {
    return t_.Find(b_);
}

size_t HuffmanTable::Size() {
    return t_.Size();
}
//copy implementation from jpeg hw