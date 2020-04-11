#pragma once

#include <algorithm>
#include <cctype>
#include <iostream>
#include <memory>
#include <string>
#include <queue>
// TODO: use hash version
#include <map>
#include <set>
#include <utility>
#include <vector>

namespace aho_corasick {

class Match {
public:
    Match(size_t start, size_t finish, std::string keyword):
        start_(start), finish_(finish), keyword_(keyword) {}

    std::string get_keyword() const {
        return keyword_;
    }

    size_t get_start() const {
        return start_;
    }

    size_t get_finish() const {
        return finish_;
    }

private:
    size_t start_, finish_;
    // TODO: more efficient will be to store keywords in one place, here just
    // use ref to it.
    std::string keyword_;
};


// TODO: what if we will build suffix refs during parsing to optimize time to construct?
class AhoCorasickAutomaton {
public:
    struct AhoCorasickAutomatonNode {
        std::unordered_map<char, std::unique_ptr<AhoCorasickAutomatonNode>> childs;
        bool is_final = false;
        // This should be initialized with ptr to root.
        // TODO: may be better to initialize it with nullptr,
        // which will mean ref to root?
        AhoCorasickAutomatonNode* suffix_ref;
        // TODO: obvious optimization: store all keywords in some place once, here
        // store only indexes of relevant keywords for particaular node.
        // TODO: bench hash version just for fun
        std::set<std::string> keywords;

        void add_keyword(const std::string& keyword) {
            keywords.insert(keyword);
        }
    };

    AhoCorasickAutomaton(): root_(std::make_unique<AhoCorasickAutomatonNode>()) {
        root_->suffix_ref = root_.get();
    }

    void insert_pattern(const std::string& s) {
        // TODO: can we avoid calling get() and work with unique_ptr directly?
        AhoCorasickAutomatonNode *node = root_.get();
        for (const auto& symbol : s) {
            if (node->childs.count(symbol) == 0) {
                node->childs[symbol] = std::make_unique<AhoCorasickAutomatonNode>();
                node->childs[symbol]->suffix_ref = root_.get();
                ++size_;
            }
            node = node->childs[symbol].get();
        }
        node->is_final = true;
        node->add_keyword(s);
        constructed_suffix_refs_ = false;
    }

    size_t size() const {
        return size_;
    }

    bool contains(std::string_view s) const {
        AhoCorasickAutomatonNode *node = root_.get();
        for (const auto& symbol: s) {
            if (node->childs.count(symbol) == 0) {
                return false;
            }
            node = node->childs[symbol].get();
        }
        return node->is_final;
    }

    std::vector<Match> parse_text(std::string_view text) {
        if (!constructed_suffix_refs_) {
            construct_suffix_refs();
            constructed_suffix_refs_ = true;
        }

        // TODO: can we avoid calling get() and work with unique_ptr directly?
        AhoCorasickAutomatonNode *curr_node = root_.get();

        size_t pos = 0;
        std::vector<Match> collected_matches;
        for (const auto& symbol : text) {
            auto res = next_node(curr_node, symbol);
            while (res == nullptr) {
                curr_node = curr_node->suffix_ref;
                res = next_node(curr_node, symbol);
                // Warning: seems that this can loop forever!!
            }
            curr_node = res;
            for (const auto& k : curr_node->keywords) {
                collected_matches.emplace_back(pos - k.size() + 1, pos, k);
            }
            ++pos;
        }
        return collected_matches;
    }

private:
    std::unique_ptr<AhoCorasickAutomatonNode> root_;
    size_t size_ = 1;
    bool constructed_suffix_refs_ = false;

    AhoCorasickAutomatonNode* next_node(AhoCorasickAutomatonNode* node, char symbol) {
        if (node->childs.count(symbol) == 1) {
            return node->childs[symbol].get();
        }
        if (node == root_.get()) {
            return root_.get();
        }
        return nullptr;
    }

    void construct_suffix_refs() {
        std::queue<AhoCorasickAutomatonNode*> q;
        q.push(root_.get());

        while (!q.empty()) {
            auto curr_node = q.front();
            for (const auto& [symbol, node] : curr_node->childs) {
                q.push(node.get());
                node->suffix_ref = get_suffix_ref(curr_node, symbol);
                for (const auto& k : node->suffix_ref->keywords) {
                    node->add_keyword(k);
                }
            }
            q.pop();
        }
    }

    // this goes up to the first good suffix ref or to the root if such no present
    //
    // And jumps then jump to suffix ref if available?
    AhoCorasickAutomatonNode* get_suffix_ref(AhoCorasickAutomatonNode* node, char symbol) {
        if (node == root_.get()) {
            return node;
        }

        auto res = node->suffix_ref;
        while (next_node(res, symbol) == nullptr) {
            res = res->suffix_ref;
        }
        return next_node(res, symbol);
    }
};

} // namespace aho_corasick
