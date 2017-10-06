#pragma once

void BubbleSort(std::vector<int>& v) {
    for (size_t i = 1; i < v.size(); ++i) {
        if (v[i] < v[i - 1])
            std::swap(v[i], v[i - 1]);
    }
}
