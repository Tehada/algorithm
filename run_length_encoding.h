#include <iostream>
#include <string>
#include <vector>

void Encode(std::string& str) {
    size_t r = 0, w = 0;
    while (r < str.size()) {
        size_t r_anchor = r;
        while (r < str.size() && str[r] == str[r_anchor])
            ++r;
        auto diff = r - r_anchor;
        str[w] = str[r_anchor];
        ++w;
        if (diff > 1) {
            auto number = std::to_string(diff);
            str.replace(w, number.size(), number);
            w += number.size();
        }
    }
    str.resize(w);
}

int main() {
    std::vector<std::string> strings = { "aabbbc", "aaaaaabbccccccdfffff", "a", "bccaaa" };
    std::vector<std::string> strings_copy = strings;
    for (auto i = 0; i < strings.size(); ++i) {
        Encode(strings[i]);
        std::cout << strings_copy[i] << " : " << strings[i] << "\n";
    }
    return 0;
}
