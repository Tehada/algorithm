#include "graph.h"

void dfs(Graph g, Vertex s) {
    s.color = GREY;
    for (auto&& node : s.nodes) {
        if (node.color == WHITE)
            dfs(node);
    }
    s.color = BLACK;
}