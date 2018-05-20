#include <string>
#include <vector>

void rle(std::string& str) {
    size_t read_ptr = 0, write_ptr = 0;
    while (read_ptr < str.size()) {
        size_t read_anchor = read_ptr;
        while (read_ptr < str.size() && str[read_ptr] == str[read_anchor])
            ++read_ptr;
        auto diff = read_ptr - read_anchor;
        str[write_ptr] = str[read_anchor];
        ++write_ptr;
        if (diff > 1) {
            auto number = std::to_string(diff);
            str.replace(write_ptr, number.size(), number);
            write_ptr += number.size();
        }
    }
    str.resize(write_ptr);
}
