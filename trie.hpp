#include <string_view>
#include <unordered_map>
#include <vector>
#include <memory>

namespace trie {

// TODO: add feature allowing allocating elements using different
// policies. Add some general predefined policies. Also make it possible
// to write custom policy according to application logic. Also write
// analyzer which will try to produce optimal policy.

class Trie {
public:
    struct TrieNode {
        std::unordered_map<char, std::unique_ptr<TrieNode>> childs;
        bool is_final = false;
    };

    Trie(): root_(std::make_unique<TrieNode>()) {}

    void insert(std::string_view s) {
        // TODO: can we avoid calling get() and work with unique_ptr directly?
        TrieNode *node = root_.get();
        for (const auto& symbol : s) {
            if (node->childs.count(symbol) == 0) {
                node->childs[symbol] = std::make_unique<TrieNode>();
                ++size_;
            }
            node = node->childs[symbol].get();
        }
        node->is_final = true;
    }

    size_t size() const {
        return size_;
    }

    bool contains(std::string_view s) {
        TrieNode *node = root_.get();
        for (const auto& symbol: s) {
            if (node->childs.count(symbol) == 0) {
                return false;
            }
            node = node->childs[symbol].get();
        }
        return node->is_final;
    }

    bool check_concat(std::string_view s) {
        TrieNode *node = root_.get();
        bool last_node_was_final = false;
        for (const auto& symbol: s) {
            if (node->childs.count(symbol) == 0) {
                if (!last_node_was_final) {
                    return false;
                }
                node = root_.get();
                if (node->childs.count(symbol) == 0) {
                    return false;
                }
            }
            node = node->childs[symbol].get();
            last_node_was_final = node->is_final;
        }
        return last_node_was_final;
    }

    friend std::ostream &operator<<(std::ostream &os, const Trie &t);

private:
    std::unique_ptr<TrieNode> root_;
    size_t size_ = 1;
};

std::ostream &operator<<(std::ostream &os, const Trie::TrieNode &n);

std::ostream &operator<<(std::ostream &os, const std::unordered_map<char, std::unique_ptr<Trie::TrieNode>> &m) {
    os << "{";
    for (const auto& elem: m) {
        os << "{" << elem.first << ": " << *(elem.second.get()) << "}, ";
    }
    os << "}";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Trie::TrieNode &n) {
    os << "<TrieNode: is_final=" << n.is_final << ", elems=\n" << n.childs << ">";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Trie &t) {
    os << "<Trie: size=" << t.size() << ", elems=\n" << *(t.root_.get());
    return os;
}

}; // namespace trie

