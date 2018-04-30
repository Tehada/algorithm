#include <iostream>
#include <vector>

template<class T>
std::vector<T> ReadVector() {
    size_t size;
    std::cin >> size;
    std::vector<T> res;
    T val;
    for (size_t i = 0; i < size; ++i) {
        std::cin >> val;
        res.push_back(val);
    }
    return res;
}

template<class T>
void PrintVector(const std::vector<T>& v) {
    std::cerr << "{";
    for (const auto& elem : v) std::cerr << elem << " ";
    std::cerr << "}\n";
}

std::vector<std::vector<int>> ReadMatrix() {
    int r, c;
    std::cin >> r >> c;
    std::vector<std::vector<int>> res(r, std::vector<int>(c));
    int n;
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            std::cin >> n;
            res[i][j] = n;
        }
    }
    return res;
}

template<class T>
void PrintMatrix(std::vector<std::vector<T>> v) {
    for (auto row : v) {
        std::cerr << "{ ";
        for (auto col : row) {
            std::cerr << col << " ";
        }
        std::cerr << "}\n";
    }
}

template<typename Map>
void PrintMap(Map& m)
{
   std::cerr << "{\n";
   for(auto& p: m)
        std::cerr << p.first << ':' << p.second << "\n";
   std::cerr << "}\n";
}
