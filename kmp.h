#include <string_view>
#include <vector>

//O(|T|)
void fillTable(std::string_view str, std::vector<size_t>* l) {
    l->resize(str.size());
    l->at(0) = 0;
    size_t len;

    for (size_t i = 0; i + 1 < str.size(); ++i) {
        len = l->at(i);
        while (str[len] != str[i + 1] && len > 0)
            len = l->at(len);
        if (str[len] == str[i + 1])
            l->at(i + 1) = len + 1;
        else
            l->at(i + 1) = 0;
    }
}

//O(|P| + |T|)
//returns index of first occurence of P in T or -1 if no occurences
ssize_t kmp(std::string_view pattern, std::string_view text) {
    std::vector<size_t> l;
    fillTable(pattern, &l);
    int j = 0, len = 0;

    while (len < pattern.size() && j < text.size()) {
        while (pattern[len] != text[j] && len > 0)
            len = l[len];
        if (pattern[len] == text[j])
            ++len;
        else
            len = 0;
        ++j;
    }
    return (len == pattern.size()) ? (j - len) : -1;
}
