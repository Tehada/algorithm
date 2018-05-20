#include <algorithm>
#include <cctype>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <queue>
#include <utility>
#include <vector>

namespace aho_corasick {

    template<class CharType>
    class BasicTrieNode {
    public:
        std::map<CharType, size_t> childs;
        bool is_final = false;
    };

    template<class CharType>
    class AhoCorasickNode {
    public:
        using string_type = std::basic_string<CharType>;

        std::map<CharType, size_t> childs;
        size_t failure_node_index;
        bool is_final = false;
        std::set<string_type> keywords;

        void AddKeyword(const string_type& keyword) {
            keywords.insert(keyword);
        }

        void AddKeywords(const std::set<string_type>& new_keywords) {
            for (const auto& keyword : new_keywords) {
                AddKeyword(keyword);
            }
        }
    };

    template<class CharType>
    class BasicTrie {
    public:
        using string_type = std::basic_string<CharType>;

        void Insert(string_type keyword) {
            if (keyword.empty())
                return;
            size_t curr_node_index = 0;
            for (const auto& symbol : keyword) {
                if (nodes_[curr_node_index].childs.count(symbol) == 0) {
                    nodes_[curr_node_index].childs[symbol] = nodes_.size();
                    nodes_.emplace_back();
                }
                curr_node_index = nodes_[curr_node_index].childs[symbol];
            }
            nodes_[curr_node_index].is_final = true;
            nodes_[curr_node_index].AddKeyword(keyword);
            //nodes_[curr_node_index].value = val;
            //for (const auto& ch : keyword) {
              //  cur_state = cur_state->add_state(ch);
            //}
            //cur_state->add_emit(keyword, d_num_keywords++);
            //d_constructed_failure_states = false;
        }

        size_t Size() const {
            return nodes_.size();
        }

        std::vector<AhoCorasickNode<CharType>> nodes_{1};
    private:
    };


    template<class CharType>
    class Match {
    public:
        using string_type = std::basic_string<CharType>;

        Match(size_t start, size_t finish, string_type keyword):
            start_(start), finish_(finish), keyword_(keyword) {}

        string_type GetKeyword() const {
            return keyword_;
        }

        size_t GetStart() const {
            return start_;
        }

        size_t GetFinish() const {
            return finish_;
        }

    private:
        size_t start_, finish_;
        string_type keyword_;
    };


    template<class CharType>
    class AhoCorasickAutomaton : public BasicTrie<CharType> {
    public:
        using string_type = std::basic_string<CharType>;

        void InsertPattern(string_type keyword) {
            BasicTrie<CharType>::Insert(keyword);
            constructed_failure_states_ = false;
        }

        std::vector<Match<CharType>> ParseText(string_type text) {
            check_construct_failure_states();
            size_t pos = 0;
            ssize_t curr_node_index = ROOT_;
            std::vector<Match<CharType>> collected_matches;
            for (auto c : text) {
                auto res = NextNodeIndex(curr_node_index, c);
                while (res == -1) {
                    curr_node_index = BasicTrie<CharType>::nodes_[curr_node_index].failure_node_index;
                    res = NextNodeIndex(curr_node_index, c);
                }
                curr_node_index = res;
                for (const auto& keyword : BasicTrie<CharType>::nodes_[curr_node_index].keywords) {
                    collected_matches.emplace_back(pos - keyword.size() + 1, pos, keyword);
                }
                ++pos;
            }
            return collected_matches;
        }

        ssize_t NextNodeIndex(size_t node_index, CharType symbol) {
            if (BasicTrie<CharType>::nodes_[node_index].childs.count(symbol) == 1) {
                return BasicTrie<CharType>::nodes_[node_index].childs[symbol];
            } else if (node_index == ROOT_) {
                return ROOT_;
            }
            return -1;
        }

        void check_construct_failure_states() {
            if (!constructed_failure_states_) {
                construct_failure_states();
            }
        }

        void construct_failure_states() {
            std::queue<size_t> q;
            for (const auto& [symbol, index] : BasicTrie<CharType>::nodes_[ROOT_].childs) {
                BasicTrie<CharType>::nodes_[index].failure_node_index = ROOT_;
                q.push(index);
            }
            constructed_failure_states_ = true;

            while (!q.empty()) {
                auto curr_node_index = q.front();
                for (const auto& [symbol, index] : BasicTrie<CharType>::nodes_[curr_node_index].childs) {
                    size_t target_node_index = NextNodeIndex(curr_node_index, symbol);
                    q.push(target_node_index);

                    size_t trace_failure_index = BasicTrie<CharType>::nodes_[curr_node_index].failure_node_index;
                    while (NextNodeIndex(trace_failure_index, symbol) == -1) {
                        trace_failure_index = BasicTrie<CharType>::nodes_[trace_failure_index].failure_node_index;
                    }
                    size_t new_failure_index = NextNodeIndex(trace_failure_index, symbol);
                    BasicTrie<CharType>::nodes_[target_node_index].failure_node_index = new_failure_index;
                    BasicTrie<CharType>::nodes_[target_node_index].AddKeywords(BasicTrie<CharType>::nodes_[new_failure_index].keywords);
                }
                q.pop();
            }
        }

    private:
        bool constructed_failure_states_ = false;
        size_t ROOT_ = 0;
    };

    using ACautomaton = AhoCorasickAutomaton<char>;
    using WACautomaton = AhoCorasickAutomaton<wchar_t>;

}
