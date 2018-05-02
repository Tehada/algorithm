#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

struct Vertex {
    size_t color;
    size_t cost;
};

void Add_edge(std::vector<std::vector<size_t>>& adjacency_matrix,
              std::vector<Vertex>& vertices, size_t vertex_begin, size_t vertex_end) {
    if (vertices[vertex_begin].color == 1) {
        std::swap(vertex_begin, vertex_end);
    }
    adjacency_matrix[0][vertex_begin] = vertices[vertex_begin].cost;
    adjacency_matrix[vertex_begin][vertex_end] = std::numeric_limits<int>::max();
    adjacency_matrix[vertex_end][number_of_vertices + 1] = vertices[vertex_end].cost;
}

std::vector<std::vector<size_t>> Input() {
    size_t number_of_vertices, number_of_edges;
    std::cin >> number_of_vertices >> number_of_edges;
    std::vector<Vertex> vertices;
    std::vector<std::vector<size_t>> adjacency_matrix;
    std::vector<size_t> string(number_of_vertices + 2);
    adjacency_matrix.resize(number_of_vertices, string);
    Vertex start_vertex;
    start_vertex.color = 0;
    start_vertex.cost = 0;
    vertices.push_back(start_vertex);
    size_t color, cost;
    for (size_t index = 1; index != number_of_vertices + 1; ++index) {
        std::cin >> color >> cost;
        Vertex input_vertex;
        input_vertex.color = color;
        input_vertex.cost = cost;
        vertices.push_back(input_vertex);
    }
    Vertex finish_vertex;
    finish_vertex.color = 1;
    finish_vertex.cost = 0;
    vertices.push_back(finish_vertex);
    size_t vertex_begin, vertex_end;
    for (size_t index = 0; index != number_of_edges; ++index) {
        std::cin >> vertex_begin >> vertex_end;
        if (vertices[vertex_begin].color != vertices[vertex_end].color) {
            Add_edge(adjacency_matrix, vertices, vertex_begin, vertex_end);
        }
    }
    return adjacency_matrix;
}

bool BFS(std::vector<std::vector<size_t>>& residual_graph, std::vector<int>& parents,
    size_t start, size_t finish) {
    std::vector<bool> visited(residual_graph.size());
    std::queue<size_t> queue;
    queue.push(start);
    visited[start] = true;
    parents[start] = -1;
    while (!queue.empty()) {
        size_t temp_vertex = queue.front();
        queue.pop();
        for (size_t vertex = 0; vertex != residual_graph.size(); ++vertex) {
            if (visited[vertex] == false && residual_graph[temp_vertex][vertex] > 0) {
                queue.push(vertex);
                parents[vertex] = temp_vertex;
                visited[vertex] = true;
            }
        }
    }
    return visited[finish] == true;
}

size_t Fulkerson(std::vector<std::vector<size_t>>& adjacency_matrix, size_t start, size_t finish) {
    std::vector<std::vector<size_t>> residual_graph = adjacency_matrix;
    std::vector<int> parents(adjacency_matrix.size());
    size_t max_flow = 0;
    while (BFS(residual_graph, parents, start, finish)) {
        size_t min_flow = std::numeric_limits<int>::max();
        for (size_t vertex = finish; vertex != start; vertex = parents[vertex]) {
            min_flow = std::min(min_flow, residual_graph[parents[vertex]][vertex]);
        }
        for (size_t vertex = finish; vertex != start; vertex = parents[vertex]) {
            residual_graph[vertex][parents[vertex]] += min_flow;
            residual_graph[parents[vertex]][vertex] -= min_flow;
        }
        max_flow += min_flow;
    }
    return max_flow;
}

int main() {
    std::vector<std::vector<size_t>> adjacency_matrix = Input();
    std::cout << Fulkerson(adjacency_matrix, 0, adjacency_matrix.size() - 1) << "\n";
}
