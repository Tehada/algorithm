#include <algorithm>
#include <iostream>
#include <vector>


bool DFS(int vertex, const std::vector<std::vector<int>>& adj_list,
         const std::vector<int>& matched, size_t n_vertices_left,
         std::vector<bool>* visited, std::vector<int>* ans) {
    visited->at(vertex) = true;

    for (auto neighbour : adj_list[vertex]) {
        if (visited->at(neighbour)) continue;

        if (matched[neighbour] == 0) {
            if (neighbour > n_vertices_left) neighbour -= n_vertices_left;
            ans->push_back(neighbour);
            ans->push_back(vertex);
            return true;
        } else if (matched[neighbour] != vertex) {
            visited->at(neighbour) = true;
            if (DFS(matched[neighbour], adj_list, matched, n_vertices_left, visited, ans)) {
                if (neighbour > n_vertices_left) neighbour -= n_vertices_left;
                ans->push_back(neighbour);
                ans->push_back(vertex);
                return true;
            }
        }
    }
    return false;
}

void PrintAnswer(const std::vector<int>& ans) {
    std::cout << ans.size() << std::endl;
    if (ans.size() > 0) {
        std::cout << ans.front();
        for (size_t i = 1; i < ans.size(); ++i) std::cout << " " << ans[i];
        std::cout << std::endl;
    }
}

int main() {
    size_t n_vertices_left, n_vertices_right, n_edges;
    std::vector<int> matched;
    std::vector<std::vector<int>> adj_list;

    const auto ReadInput = [&]() {
        std::cin >> n_vertices_left >> n_vertices_right >> n_edges;
        std::pair<int, int> edge;
        adj_list.resize(n_vertices_left + n_vertices_right + 1);
        for (size_t i = 0; i < n_edges; ++i) {
            std::cin >> edge.first >> edge.second;
            edge.second += n_vertices_left;
            adj_list[edge.first].push_back(edge.second);
            adj_list[edge.second].push_back(edge.first);
        }
        size_t matching_size;
        std::cin >> matching_size;
        matched.resize(n_vertices_left + n_vertices_right + 1, 0);
        for (size_t i = 0; i < matching_size; ++i) {
            std::cin >> edge.first >> edge.second;
            edge.second += n_vertices_left;
            matched[edge.first] = edge.second;
            matched[edge.second] = edge.first;
        }
    };


    ReadInput();

    std::vector<bool> visited(n_vertices_left + n_vertices_right + 1, false);
    std::vector<int> ans;
    for (int vertex = 1; vertex <= n_vertices_left; ++vertex) {
        if (matched[vertex] == 0) {
            if (DFS(vertex, adj_list, matched, n_vertices_left, &visited, &ans)) {
                break;
            }
        }
    }

    std::reverse(ans.begin(), ans.end());

    PrintAnswer(ans);
    return EXIT_SUCCESS;
}
