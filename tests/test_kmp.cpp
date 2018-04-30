#include <iostream>

#include "../kmp.h"

int main() {
    std::string pattern = "abz";
    std::string text = "alkfabzcasde";
    auto res = kmp(pattern, text);
    std::cout << "kmp: " << res << "\nfind: " << text.find(pattern) << "\n";
}
