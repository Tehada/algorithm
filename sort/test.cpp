#include <algorithm>
#include <iostream>

#include <utils.h>
#include <sort.h>

void Test() {
    std::vector<int> v{2, 5, 1, 6, 4, 3, 7};
    std::vector<int> u = v;
    BubbleSort(v);
    std::sort(u.begin(), u.end());
    if (v != u)
        std::cout << "Not equal\n";
}

//how to return a permutation of vector instead of swaping contents?

int main() {
    Test();
}
