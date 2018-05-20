#include <algorithm>
#include <iostream>
#include <vector>


struct Solution {
    int weight, cost;//enough?
    std::vector<int> items;

    //bool operator>=(auto lhs, auto rhs) {
      //  return (lhs.weight >= rhs.weight) && (lhs.cost <= rhs.cost);
    //}

    Solution(int cost_, int weight_): cost(cost_), weight(weight_) {}
};

struct Item {
    int cost, weight, id;
};

void ReadInput(int& weight, std::vector<Item>* items) {
    int n_items;
    std::cin >> n_items >> weight;
    Item item;
    items->reserve(n_items);
    for (int i = 0; i < n_items; ++i) {
        std::cin >> item.cost >> item.weight;
        item.id = i;
        items->push_back(item);
    }
}

void Solve(const std::vector<Item>& items, int weight, std::vector<Solution>& s) {
    //Solution solution;

    const auto PrintSolutions = [&] () {
        std::cerr << "solutions:\n";
        for (auto&& elem : s) {
            std::cout << elem.weight << " " << elem.cost << std::endl;
        }
    };

    s.emplace_back(0, 0);
    s.back().items.resize(items.size(), false);
    for (size_t i = 0; i < items.size(); ++i) {
        auto size = s.size();
        for (size_t j = 0; j < size; ++j) {
            if (s[j].weight + items[i].weight <= weight) {
                s.emplace_back(s[j].cost + items[i].cost, s[j].weight + items[i].weight);
                s.back().items = s[j].items;
                s.back().items[i] = true;
            }
        }
        std::sort(s.begin(), s.end(), [](auto lhs, auto rhs) {
            if (lhs.weight == rhs.weight) return lhs.cost < rhs.cost;
            else return lhs.weight < rhs.weight;
        });
        for (auto it = s.begin(); it + 1 < s.end(); ++it) {
            if (it->cost >= (it + 1)->cost) s.erase(it);
        }
        std::cout << "size: " << s.size() << "\n";
        //PrintSolutions();
    }
}

int main() {
    int weight;
    std::vector<Item> items;
    ReadInput(weight, &items);
    std::vector<int> ans;
    std::vector<Solution> s;
    s.reserve(20000);
    Solve(items, weight, s);
    std::cout << s.back().weight << "\n";
    //PrintAnswer();
}
