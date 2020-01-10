#include <vector>

#include "util.hpp"

void cocktail_shaker_sort(std::vector<int>& v) {}

void comb_sort(std::vector<int>& v) {}

void selection_sort(std::vector<int>& v) {}

void insertion_sort(std::vector<int>& v) {}

// TODO: rewrite for iterstors
void bubble_sort(std::vector<int>& v) {
    size_t n = v.size();
    while (n > 0) {
        size_t index_of_last_swapped = 0;
        for (size_t i = 1; i < n; ++i) {
            if (v[i] < v[i - 1]) {
                std::swap(v[i], v[i - 1]);
                index_of_last_swapped = i;
            }
        }
        n = index_of_last_swapped;
    }
}

