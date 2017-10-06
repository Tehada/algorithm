#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <queue>
#include <unordered_map>
#include <unordered_set>

struct Vertex {
    size_t id = 0;
    std::string color = "white";
    size_t distance = 0;
    Vertex parent = nullptr;

    bool mark = false;
    size_t root = 0;

    Vertex() {};
};

struct Edge {
    std::pair<Vertex, Vertex> edge;
    bool match = false;
    bool mark = false;

    Edge(std::pair<Vertex, Vertex> new_edge, bool new_mark) : edge(new_edge), mark(new_mark) {};
};

struct Link {
    Vertex vertex;
    Link* next;
    Link* prev;
};

class CycleList {
  public:
    CycleList() {
        first = nullptr;
        last  = nullptr;
    }

    ~CycleList() {
        while(first != last) {
            Link* temp = first;
            first = first -> prev;
            delete temp;
        }
        delete last;
    }

    void push_back(Vertex v) {
        Link* new_link = new Link;
        new_link -> vertex = v;
        if (first) {
            new_link -> prev = first;
            first -> next =  new_link;
        } else {
            last = new_link;
        }
        new_link -> next = last;
        last -> prev = new_link;
        first = new_link;
        ++size;
    }

    size_t size() {
        return size;
    }

    Iterator begin() {

    }

    Iterator end() {
        
    }


    /*
    void pushfront( char* str) {
    Link* newlink = new Link;
    strcpy(newlink ->word, str);
 
    if(last)
    {
          newlink ->next = last;
          last ->prev =  newlink;
    } else {
        first = newlink;
    }
 
    newlink ->prev = first;
    first ->next = newlink;
    last = newlink;
    }
    */

  private:
    Link* first;
    Link* last;
    size_t size = 0;
};

//TODO: переписать все места, где Edge

class Graph {
  public:
    //using Vertex = size_t;
    using VertexSet = std::unordered_set<Vertex>;
    using EdgeSet = std::unordered_set<std::pair<Vertex, Vertex>>;
    using EdgeMap = std::unordered_map<Vertex, EdgeSet>;
    using AdjacencyList = std::unordered_map<size_t, VertexSet>;

    void AddVertex(Vertex v) {
        adjacency_list_[v.id];
    }

    void AddEdge(Vertex u, Vertex v) {
        adjacency_list_[u.id].insert(v);
        adjacency_list_[v.id].insert(u);
        std::pair<Vertex, Vertex> new_edge = { u, v };
        edges_.insert(new_edge);
    }

    const VertexSet& AdjacentVertices(Vertex v) const {
        const auto it = adjacency_list_.find(v.id);
        if (it != adjacency_list_.end()) {
            return it->second;
        } else {
            return empty_set_;
        }
    }

    VertexSet AllVertices() const {
        VertexSet vs;
        vs.reserve(adjacency_list_.size());
        for (const auto& pair : adjacency_list_) {
            const auto& vertex = pair.first;
            vs.insert(vertex);
        }
        return vs;
    }

    EdgeSet AllEdges() const {
        return edges_;
    }

    //Я это использую???
    const AdjacencyList& AsAdjacencyList() const {
        return adjacency_list_;
    }

    bool IsInGraph(Vertex v) {
        id = v.id;
        for (auto pair : adjacency_list_) {
            auto vertex = pair->first;
            if (vertex.id == id) {
                return true;
            }
        }
        return false;
    }

    size_t Distance(Vertex start, Vertex finish) {
        size_t v = start.id, u = finish.id;
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
            v = queue.pop();
            for (auto neighbor : graph.AdjacentVertices(v)) {
                if (neighbor.color == "white") {
                    neighbor.color = "gray";
                    neighbor.distance = v.distance + 1;
                    neighbor.parent = v;
                    queue.push(neighbor);
                }
                if (neighbor.id == u) {
                    return neighbor.distance;
                }
            }
            v.color = "black";
        }
    }

  private:
    AdjacencyList adjacency_list_;
    static const VertexSet empty_set_;
    EdgeSet edges_;
};

class Matching {
  public:
    Matching(Graph initial_graph) : graph_(initial_graph) {
        for (auto vertex : graph_.AllVertices()) {
            if (!IsInMatching(vertex)) {
                for (auto neighbour : graph_.AdjacentVertices(vertex)) {
                    if (!IsInMatching(neighbour)) {
                        vertex_cover_.insert(vertex);
                        vertex_cover_.insert(neighbour);
                        std::pair<Vertex, Vertex> new_edge = { vertex, neighbour };
                        edge_cover_.insert(new_edge);
                        break;
                    }
                }
            }
        }
    }

    void AddEdge(Vertex v, Vertex u) {
        for (auto edge : graph_.AllEdges()) {
            if (edge.first == v && edge.second == u ||
                edge.first == u && edge.second == v) {
                edge.match = true;
            }
        }
    }

    void RemoveEdge(size_t v, size_t u) {

    }

    bool IsInMatching(std::pair<size_t, size_t> edge) {

    }

    bool IsInMatching(Vertex vertex) {
        if (vertex_cover_.find(vertex) != vertex_cover_.end()) {
            return true;
        }
        return false;
    }

    size_t MatchingSize() {
        return edge_cover_.size();
    }

    Graph& GetGraph() {
        return graph_;
    }

    size_t FindMatched(size_t vertex) {
        for (auto pair : edge_cover_) {
            if (vertex == pair.first) {
                return pair.second;
            } else if (vertex == pair.second) {
                return pair.first;
            }
        }
    }

    void BFS(std::queue<Vertex> queue) {
        //size_t v = start.id, u = finish.id;
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
            v = queue.pop();
            for (auto neighbor : graph.AdjacentVertices(v)) {
                if (neighbor.color == "white") {
                    neighbor.color = "gray";
                    neighbor.distance = v.distance + 1;
                    neighbor.parent = v;
                    queue.push(neighbor);
                }
                if (neighbor.id == u) {
                    return neighbor.distance;
                }
            }
            v.color = "black";
        }
    }

    std::list<Vertex> FindBetterMatching() {
        Graph forest;
        for (auto vertex : graph_.AllVertices()) {
            vertex.mark = false;
        }
        for (auto edge : graph_.AllEdges()) {
            edge.mark = false;
        }
        for (auto edge : edge_cover_) {
            edge.mark = true;
        }
        for (auto vertex : graph.AllVertices()) {
            if (vertex_cover_.find(vertex) == vertex_cover_.end()) {
                vertex.root = vertex.id;
                forest.AddVertex(vertex);
            }
        }
        //Create special set for unmarked, too much memory, we can only id remember!
        Graph::VertexSet unmarked_vertices;
        for (auto vertex : forest.AllVertices()) {
            unmarked_vertices.insert(vertex);
        }
        Graph::EdgeMap unmarked_forest_edges;
        for (auto vertex : forest.AllVertices()) {
            for (auto neighbor : graph.AdjacentVertices(vertex)) {
                std::pair<size_t, size_t> new_edge = { vertex, neighbor };
                Edge new_edge(new_edge, false);
                unmarked_forest_edges[vertex].insert(new_edge);
            }
        }
        for (auto vertex : unmarked_vertices) {
            if (Graph::Distance(vertex.root, vertex) % 2 == 0) {
                for (auto edge : unmarked_forest_edges[vertex]) {
                    size_t next_vertex;
                    if (edge.first == vertex.id) {
                        next_vertex = edge.second;
                    } else {
                        next_vertex = edge.first;
                    }
                    if (!forest.IsInGraph(next_vertex)) {
                        size_t x = this.FindMatched(next_vertex);
                        forest.AddEdge(vertex, next_vertex);
                        forest.AddEdge(next_vertex, x);
                    } else {
                        if (Graph::Distance(next_vertex.root, next_vertex) % 2 != 0) {
                            continue;
                        } else {
                            std::list<Vertex> path;
                            path.push_front(vertex);
                            while (vertex != vertex.root) {
                                vertex = vertex.parent;
                                path.push_front(vertex);
                            }
                            path.push_back(next_vertex);
                            while (next_vertex != next_vertex.root) {
                                next_vertex = next_vertex.parent;
                                path.push_back(next_vertex);
                            }
                            if (vertex.root != next_vertex.root) {
                                return path;
                            } else {
                                Vertex begin = path.pop_front();
                                Vertex end = path.pop_back();
                                Vertex root;
                                while (begin == end) {
                                    root = begin;
                                    begin = path.pop_front();
                                    end = path.pop_back();
                                }
                                CycleList cycle;
                                cycle.push_back(root);
                                cycle.push_back(begin);
                                while (begin != end) {
                                    begin = path.pop_front();
                                    cycle.push_back(begin);
                                }

                                //составить массив с вершинами из цикла
                                //закинуть их в правильном порядке в CycleList
                                //Найти base цикла
                                //заустить bfs для вершин цикла
                                //если найдем свободную вершину где-нибудь, то:
                                //проложить путь от неё до цикла, в зависимости от четности,
                                //пойти вдоль цикла до base, а от него до начала
                                //вернуть путь!!
                            }
                        }
                    }
                    //тут где-то маркировки назначаются
                }
            }
        }
    }

    void RelaxPath(path) {
        //easy realization:
        for (auto it = path.begin(); it != path.end() - 2; it += 2) { // -1 or -2 ??
            edge_cover_.AddEdge(*it, *(it + 1));
            edge_cover_.RemoveEdge(*(it + 1), *(it + 2));
        }
        edge_cover_.AddEdge(*it, *(it + 1));
        vertex_cover_.AddVertex(*path.begin());
        vertex_cover_.AddVertex(*(path.end() - 1));
    }

    void Edmonds() {
        while (true) {
            path = edge_cover_.FindBetterMatching();
            if (path.empty()) {
                break;
            }
            edge_cover_.RelaxPath(path);
        }
    }

    //something else????

  private:
    Graph& graph_;
    Graph::VertexSet vertex_cover_;
    //edgecover , кажеться, не нужен
    Graph::EdgeSet edge_cover_;
};

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

void GraphEdges(std::ostream& out, const Graph::AdjacencyList& adjacency_list) {
    for (const auto& pair : adjacency_list) {
        const auto& vertex = pair.first;
        const auto& neighbours = pair.second;
        for (const auto adj_vertex : neighbours) {
            out << "\t" << vertex << " -- " << adj_vertex << "\n";
        }
    }
}

// Use http://www.webgraphviz.com to take a look at the graph
void GraphViz(std::ostream& out, const Graph& graph) {
    out << "strict graph {\n";
    for (const auto& pair : graph.AsAdjacencyList()) {
        const auto& vertex = pair.first;
        out << "\t" << vertex << "\n";
    }
    GraphEdges(out, graph.AsAdjacencyList());
    out << "}\n";
}

void GraphViz(std::ostream& out, const Matching& edge_matching) {
    out << "graph {\n";
    for (auto pair : edge_matching) {
        out << "\t" << pair.first << " -- " << pair.second << "[color=red,penwidth=3.0]\n";
    }
    /*
    for (const auto& pair : vertex_path.GetGraph().AsAdjacencyList()) {
        const auto& vertex = pair.first;
        if (vertex_path.find(vertex) != vertex_path.end()) {
            out << "\t" << vertex << " [shape=triangle, color=red]\n";
        } else {
            out << "\t" << vertex << "\n";
        }
    }
    */
    GraphEdges(out, vertex_path.GetGraph().AsAdjacencyList());
    out << "}\n";
}

struct DebugInfo {
    std::vector<size_t> costs;
};

Graph RandomGraph(size_t size, double edge_probability) {
    Graph graph;
    for (size_t id = 1; id <= size; ++id) {
        Vertex new_vertex;
        new_vertex.id = id;
        new_vertex.color = "white";
        new_vertex.distance = std::numeric_limits<size_t>::max();
        new_vertex.parent = nullptr;
        graph.AddVertex(new_vertex);
    }
    for (size_t v = 1; v <= size; ++v) {
        for (size_t u = v + 1; u <= size; ++u) {
            if (double(rand()) / RAND_MAX <= edge_probability) {
                graph.AddEdge(v, u);
            }
        }
    }
    return graph;
}

int InitRandSeed(int argc, const char* argv[]) {
    int rand_seed;
    if (argc >= 2) {
        rand_seed = atoi(argv[1]);
    } else {
        rand_seed = time(nullptr);
    }
    srand(rand_seed);
    return rand_seed;
}

void TrySolver(const PathSolver& solver, const Graph& graph) {
    GraphViz(std::cout, graph);
    auto best_cost = 0;
    size_t results = 0;
    DebugInfo out;
    for (int attempt = 1; attempt < 100; ++attempt) {
        DebugInfo debug_info;
        const auto path = solver.Solve(graph, debug_info);
        auto cost = path.Cost();
        if (cost > best_cost) {
            best_cost = cost;
            GraphViz(std::cout, path);
            out = debug_info;
            //std::cout << "Trace info:\n" << debug_info << "\n";
            ++results;
        }
    }
    std::cout << out << "\n";
    std::cout << "Results: " << results << std::endl;
}

/*
Matching Edmonds(Graph& graph, Matching& edge_cover) {
    while (true) {
        path = edge_cover.FindBetterMatching();
        if (path.empty()) {
            break;
        }
        edge_cover.RelaxPath(path);
    }
    return edge_cover;
}
*/

int main(int argc, const char* argv[]) {
    int seed = InitRandSeed(argc, argv);
    const auto graph = RandomGraph(7, 0.5);
    Matching edge_cover(graph);
    std::cout << "Greedy matching size: " << edge_cover.MatchingSize() << "\n";
    GraphViz(std::cout, edge_cover);
    Matching max_matching = Edmonds(edge_cover);
    std::cout << "Edmonds matching size: " << max_matching.MatchingSize() << "\n";
    GraphViz(std::cout, edge_cover);
}
