#include <algorithm>
#include <iostream>
#include <vector>
#include <utility>

template <typename T>
class Dsu {
public:
    Dsu(size_t count, std::vector<int> mark) : set_count(count), mark(mark) {
        for (size_t i = 0; i != set_count; ++i) {
            parent.push_back(i);
            rank.push_back(0);
        }
    }
    int find(int elem) {
        if (parent[elem] == elem) {
            return parent[elem];
        } else {
            return find(parent[elem]);
        }
    }
    bool are_joined(const int& first, const int& second){
        if (find(first) == find(second)) {
            return true;
        }
        return false;
    }

    bool marked(int elem) {
        if (std::find(mark.begin(), mark.end(), elem) != mark.end()) {
            return true;
        }
        return false;
    }

    bool join(int first, int second) {
        int set_first = find(first);
        int set_second = find(second);
        if (set_first == set_second) {
            return false;
        } else if (marked(set_first) && marked(set_second)) {
            return false;
        } else {
            if (marked(set_first)) {
                parent[set_second] = set_first;
            } else if (marked(set_second)) {
                parent[set_first] = set_second;
            } else {
                parent[set_first] = set_second;
            }
            --set_count;
            return true;
        }
    }
    const size_t size() {
        return parent.size();
    }
    const int disjoint_sets_count() {
        return set_count;
    }
private:
    int set_count;
    std::vector<int> parent;
    std::vector<int> rank;
    std::vector<int> mark;
};

struct edge {
    int weight;
    int begin;
    int end;
    edge(int weight, int begin, int end)
        : weight(weight), begin(begin), end(end) {}
};

void input(size_t& height, size_t& length, std::vector<int>& mark,
           std::vector<edge>& edges) {
    int marker, right, down;
    if (height == 1 && length == 1) {
        std::cin >> marker;
        if (marker == 1) {
            mark.push_back(1);
        }
    } else if (height == 1) {
        for (size_t i = 0; i != length - 1; ++i) {
            std::cin >> marker >> right;
            if (marker == 1) {
                mark.push_back(i);
            }
            edge right_edge(right, i, i + 1);
            edges.push_back(right_edge);
        }
        std::cin >> marker;
        if (marker == 1) {
            mark.push_back(length - 1);
        }
    } else if (length == 1) {
        for (size_t i = 0; i != height - 1; ++i) {
            std::cin >> marker >> down;
            if (marker == 1) {
                mark.push_back(i);
            }
            edge down_edge(down, i, i + 1);
            edges.push_back(down_edge);
        }
        std::cin >> marker;
        if (marker == 1) {
            mark.push_back(height - 1);
        }
    } else {
        for (size_t i = 0; i != height - 1; ++i) {
            for (size_t j = 0; j != length - 1; ++j) {
                std::cin >> marker >> right >> down;
                if (marker == 1) {
                    mark.push_back((i + 1) * length + j);
                }
                edge right_edge(right,
                                i * length + j,
                                i * length + j + 1);
                edges.push_back(right_edge);
                edge down_edge(down,
                               i * length + j,
                               (i + 1) * length + j);
                edges.push_back(down_edge);
            }
            std::cin >> marker >> down;
            if (marker == 1) {
                mark.push_back(length - 1);
            }
            edge down_edge(down,
                (i + 1) * length - 1,
                (i + 2) * length - 1);
            edges.push_back(down_edge);
        }
        for (size_t i = 0; i != length - 1; ++i) {
            std::cin >> marker >> right;
            if (marker == 1) {
                mark.push_back((length * (height - 1)) + i);
            }
            edge right_edge(right,
                            (length * (height - 1)) + i,
                            (length * (height - 1)) + i + 1);
            edges.push_back(right_edge);
        }
        std::cin >> marker;
        if (marker == 1) {
            mark.push_back(length * height - 1);
        }
    }
}

bool compare(edge first, edge second) {
    return first.weight < second.weight;
}

void kruskal(std::vector<edge>& edges, std::vector<int>& mark, const size_t& size, size_t& total_price) {
    std::sort(edges.begin(), edges.end(), compare);
    Dsu<int> set_union(size, mark);
    for (auto edge : edges) {
        if (!set_union.are_joined(edge.begin, edge.end)
            && !(set_union.marked(edge.begin) && set_union.marked(edge.end))) {
            if (set_union.join(edge.begin, edge.end))
                total_price += edge.weight;
        }
    }
}

size_t calculate_answer() {
    size_t height, length;
    std::cin >> height >> length;
    size_t size = height * length;
    std::vector<int> mark;
    std::vector<edge> edges;
    input(height, length, mark, edges);
    size_t total_price = 0;
    kruskal(edges, mark, size, total_price);
    return total_price;
}
