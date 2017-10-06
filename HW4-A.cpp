#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

std::vector<std::pair<double, int> > input(int number) {
    std::vector<std::pair<double, int> > prices;
    for (size_t i = 0; i != number; ++i) {
        std::pair<double, int> substance;
        std::cin >> substance.first >> substance.second;
        substance.first /= substance.second;
        prices.push_back(substance);
    }
    return prices;
}

void compare() {

}

int main() {
    int number, max_weight;
    std::cin >> number >> max_weight;
    std::vector<std::pair<double, int> > prices = input(number);
    for (size_t i = 0; i != prices.size(); ++i) {
        std::cout << std::fixed << std::setprecision(6) << prices[i].first << "\n";
    }
    std::sort(prices.begin(), prices.end()) {
        return left.first > right.first;
    }
    for (size_t i = 0; i != prices.size(); ++i) {
        std::cout << std::fixed << std::setprecision(6) << prices[i].first << "\n";
    }
}
