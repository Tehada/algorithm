#include <iostream>

#include "../rle.h"

int main() {
    std::vector<std::string> strings = { "aabbbc", "aaaaaabbccccccdfffff", "a", "bccaaa" };
    std::vector<std::string> strings_copy = strings;
    for (auto i = 0; i < strings.size(); ++i) {
        rle(strings[i]);
        std::cout << strings_copy[i] << " : " << strings[i] << "\n";
    }
    return 0;
}