#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>

void Input(std::vector<std::vector<size_t>>& table, size_t number_of_states,
    size_t alphabet_size, std::set<size_t>& terminal_states) {
    size_t transition;
    for (size_t index = 0; index != number_of_states; ++index) {
        for (size_t jindex = 0; jindex != alphabet_size; ++jindex) {
            std::cin >> transition;
            table[index].push_back(transition);
        }
    }
    size_t initial_state, number_of_terminals, state;
    std::cin >> initial_state >> number_of_terminals;
    for (size_t index = 0; index != number_of_terminals; ++index) {
        std::cin >> state;
        terminal_states.insert(state);
    }
}

std::set<size_t> FindStates(size_t symbol, std::set<size_t>& set,
    std::vector<std::vector<size_t>>& table) {
    std::set<size_t> ways_to_set;
    for (size_t index = 0; index != table.size(); ++index) {
        if (std::binary_search(set.begin(),
            set.end(), table[index][symbol])) {
            ways_to_set.insert(index);
        }
    }
    return ways_to_set;
}

std::set<size_t> FindIntersection(std::set<size_t>& first_set, std::set<size_t>& second_set) {
    std::set<size_t> intersection;
    std::set_intersection(first_set.begin(), first_set.end(), second_set.begin(), second_set.end(),
        std::inserter(intersection, intersection.begin()));
    return intersection;
}

std::set<size_t> FindDifference(std::set<size_t>& first_set, std::set<size_t>& second_set) {
    std::set<size_t> difference;
    std::set_difference(first_set.begin(), first_set.end(), second_set.begin(), second_set.end(),
        std::inserter(difference, difference.begin()));
    return difference;
}

int FindSet(std::vector<std::set<size_t>>& queue, std::set<size_t>& set) {
    for (size_t index = 0; index != queue.size(); ++index) {
        if (queue[index] == set) {
            return index;
        }
    }
    return -1;
}

size_t Minimize(size_t alphabet_size, std::vector<std::vector<size_t>>& table,
    std::set<size_t>& terminal_states) {
    std::vector<std::set<size_t>> eq_classes;
    std::vector<std::set<size_t>> queue;
    std::set<size_t> all_states;
    for (size_t index = 0; index != table.size(); ++index) {
        all_states.insert(index);
    }
    std::set<size_t> next_set;
    if (!terminal_states.empty()) {
        eq_classes.push_back(terminal_states);
        eq_classes.push_back(FindDifference(all_states, terminal_states));
        queue.push_back(terminal_states);
    } else {
        eq_classes.push_back(all_states);
        queue.push_back(all_states);
    }
    while (!queue.empty()) {
        next_set = queue.back();
        queue.pop_back();
        std::set<size_t> ways_to_next;
        for (size_t symbol = 0; symbol != alphabet_size; ++symbol) {
            std::set<size_t> ways_to_next;
            ways_to_next = FindStates(symbol, next_set, table);
            for (size_t index = 0; index != eq_classes.size(); ++index) {
                std::set<size_t> intersection = FindIntersection(eq_classes[index], ways_to_next);
                std::set<size_t> difference = FindDifference(eq_classes[index], ways_to_next);
                if (!intersection.empty() && !difference.empty()) {
                    eq_classes.erase(eq_classes.begin() + index);
                    eq_classes.push_back(intersection);
                    eq_classes.push_back(difference);
                    int index_of_set = FindSet(queue, eq_classes[index]);
                    if (index_of_set != -1) {
                        queue.erase(queue.begin() + index_of_set);
                        queue.push_back(intersection);
                        queue.push_back(difference);
                    } else {
                        if (intersection.size() <= difference.size()) {
                            queue.push_back(intersection);
                        } else {
                            queue.push_back(difference);
                        }
                    }
                }
            }
        }
    }
    return eq_classes.size();
}

int main() {
    size_t number_of_states, alphabet_size;
    std::cin >> number_of_states >> alphabet_size;
    std::vector<std::vector<size_t>> table(number_of_states);
    std::set<size_t> terminal_states;
    Input(table, number_of_states, alphabet_size, terminal_states);
    std::cout << Minimize(alphabet_size, table, terminal_states) << "\n";
}
