#include <limits>
#include <queue>
#include <string>

struct Vertex {
    std::string color;
    size_t distance;
    Vertex parent;
}

void BFS(Graph graph, Vertex start) {
    for (auto vertex : graph.AllVertices()) {
        if (vertex != start) {
            vertex.color = "white";
            vertex.distance = std::numeric_limits<size_t>::max();
            vertex parent = nullptr;
        }
    }
    start.color = "gray";
    start.distance = 0;
    start.parent = nullptr;
    std::queue<Vertex> queue;
    queue.push(start);
    while (!queue.empty()) {
        vertex = queue.pop();
        for (auto neighbor : graph.AdjacentVertices(vertex)) {
            if (neighbor.color == "white") {
                neighbor.color = "gray";
                neighbor.distance = vertex.distance + 1;
                neighbor.parent = vertex;
                queue.push(neighbor);
            }
        }
        vertex.color = "black";
    }
}
