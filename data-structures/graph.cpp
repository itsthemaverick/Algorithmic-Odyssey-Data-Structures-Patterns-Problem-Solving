#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <limits>
#include <algorithm>
#include <sstream>
#include <functional>

/**
 * @brief A production-grade, generic Weighted Graph implementation using Adjacency Lists.
 * 
 * @tparam T The type used to identify vertices/nodes (e.g., std::string, int). Must be hashable.
 */
template <typename T>
class Graph {
public:
    struct Edge {
        T destination;
        double weight;

        bool operator>(const Edge& other) const {
            return weight > other.weight;
        }
    };

private:
    bool is_directed;
    std::unordered_map<T, std::vector<Edge>> adj_list;

public:
    // =========================================================================
    // Constructors & Destructor
    // =========================================================================

    /**
     * @brief Construct a new Graph instance.
     * @param directed If true, edges are directed (A -> B). If false, undirected (A <-> B).
     */
    explicit Graph(bool directed = false) : is_directed(directed) {}

    ~Graph() = default;

    // =========================================================================
    // Vertex & Edge Management
    // =========================================================================

    /**
     * @brief Adds a vertex to the graph if it doesn't already exist.
     */
    void add_vertex(const T& vertex) {
        if (adj_list.find(vertex) == adj_list.end()) {
            adj_list[vertex] = {};
        }
    }

    /**
     * @brief Adds a weighted edge between source and destination.
     */
    void add_edge(const T& src, const T& dest, double weight = 1.0) {
        add_vertex(src);
        add_vertex(dest);

        adj_list[src].push_back({dest, weight});
        if (!is_directed) {
            adj_list[dest].push_back({src, weight});
        }
    }

    /**
     * @brief Removes an edge from source to destination.
     */
    bool remove_edge(const T& src, const T& dest) {
        if (adj_list.find(src) == adj_list.end() || adj_list.find(dest) == adj_list.end()) {
            return false;
        }

        auto erase_edge = [](std::vector<Edge>& edges, const T& target) {
            auto it = std::remove_if(edges.begin(), edges.end(), [&](const Edge& e) {
                return e.destination == target;
            });
            bool found = (it != edges.end());
            edges.erase(it, edges.end());
            return found;
        };

        bool removed = erase_edge(adj_list[src], dest);
        if (!is_directed) {
            erase_edge(adj_list[dest], src);
        }
        return removed;
    }

    /**
     * @brief Removes a vertex and all incident edges connected to it.
     */
    bool remove_vertex(const T& vertex) {
        if (adj_list.find(vertex) == adj_list.end()) return false;

        // Erase the vertex entry
        adj_list.erase(vertex);

        // Erase incident edges pointing to this vertex
        for (auto& [node, edges] : adj_list) {
            auto it = std::remove_if(edges.begin(), edges.end(), [&](const Edge& e) {
                return e.destination == vertex;
            });
            edges.erase(it, edges.end());
        }
        return true;
    }

    // =========================================================================
    // Capacity & Structural Queries
    // =========================================================================

    [[nodiscard]] size_t vertex_count() const noexcept { return adj_list.size(); }
    [[nodiscard]] bool empty() const noexcept { return adj_list.empty(); }
    [[nodiscard]] bool has_vertex(const T& vertex) const { return adj_list.find(vertex) != adj_list.end(); }

    [[nodiscard]] bool has_edge(const T& src, const T& dest) const {
        if (!has_vertex(src)) return false;
        for (const auto& edge : adj_list.at(src)) {
            if (edge.destination == dest) return true;
        }
        return false;
    }

    // =========================================================================
    // Traversals (BFS & DFS)
    // =========================================================================

    /**
     * @brief Breadth-First Search traversal starting from start_node.
     */
    void bfs(const T& start_node, const std::function<void(const T&)>& visitor) const {
        if (!has_vertex(start_node)) return;

        std::unordered_set<T> visited;
        std::queue<T> q;

        visited.insert(start_node);
        q.push(start_node);

        while (!q.empty()) {
            T current = q.front();
            q.pop();

            visitor(current);

            for (const auto& edge : adj_list.at(current)) {
                if (visited.find(edge.destination) == visited.end()) {
                    visited.insert(edge.destination);
                    q.push(edge.destination);
                }
            }
        }
    }

    /**
     * @brief Depth-First Search traversal starting from start_node.
     */
    void dfs(const T& start_node, const std::function<void(const T&)>& visitor) const {
        if (!has_vertex(start_node)) return;

        std::unordered_set<T> visited;
        dfs_helper(start_node, visited, visitor);
    }

private:
    void dfs_helper(const T& current, std::unordered_set<T>& visited, const std::function<void(const T&)>& visitor) const {
        visited.insert(current);
        visitor(current);

        for (const auto& edge : adj_list.at(current)) {
            if (visited.find(edge.destination) == visited.end()) {
                dfs_helper(edge.destination, visited, visitor);
            }
        }
    }

public:
    // =========================================================================
    // Pathfinding Algorithms
    // =========================================================================

    /**
     * @brief Dijkstra's Algorithm for Single-Source Shortest Path (Non-negative weights).
     * @return Pair containing distance map and predecessor parent map for path reconstruction.
     */
    auto dijkstra(const T& start_node) const {
        std::unordered_map<T, double> distances;
        std::unordered_map<T, T> predecessors;

        for (const auto& [vertex, _] : adj_list) {
            distances[vertex] = std::numeric_limits<double>::infinity();
        }

        if (!has_vertex(start_node)) return std::make_pair(distances, predecessors);

        distances[start_node] = 0.0;

        // Priority queue storing pair<distance, vertex>
        using PQItem = std::pair<double, T>;
        std::priority_queue<PQItem, std::vector<PQItem>, std::greater<PQItem>> pq;

        pq.push({0.0, start_node});

        while (!pq.empty()) {
            auto [current_dist, current] = pq.top();
            pq.pop();

            if (current_dist > distances[current]) continue;

            for (const auto& edge : adj_list.at(current)) {
                double new_dist = distances[current] + edge.weight;
                if (new_dist < distances[edge.destination]) {
                    distances[edge.destination] = new_dist;
                    predecessors[edge.destination] = current;
                    pq.push({new_dist, edge.destination});
                }
            }
        }

        return std::make_pair(distances, predecessors);
    }

    /**
     * @brief Reconstructs shortest path to dest_node using predecessor map.
     */
    std::vector<T> reconstruct_path(const T& start_node, const T& dest_node, const std::unordered_map<T, T>& predecessors) const {
        std::vector<T> path;
        T current = dest_node;

        while (current != start_node) {
            path.push_back(current);
            auto it = predecessors.find(current);
            if (it == predecessors.end()) {
                return {}; // Path doesn't exist
            }
            current = it->second;
        }
        path.push_back(start_node);
        std::reverse(path.begin(), path.end());
        return path;
    }

    // =========================================================================
    // Advanced Graph Algorithms
    // =========================================================================

    /**
     * @brief Topological Sorting via Kahn's Algorithm (Kahn / In-degree queue).
     * @return Vector of vertices in topologically sorted order. Throws error if graph has cycle.
     */
    std::vector<T> topological_sort() const {
        if (!is_directed) {
            throw std::invalid_argument("Topological sort is only valid for Directed Acyclic Graphs (DAGs).");
        }

        std::unordered_map<T, int> in_degree;
        for (const auto& [vertex, _] : adj_list) {
            in_degree[vertex] = 0;
        }

        for (const auto& [src, edges] : adj_list) {
            for (const auto& edge : edges) {
                in_degree[edge.destination]++;
            }
        }

        std::queue<T> zero_in_degree;
        for (const auto& [vertex, degree] : in_degree) {
            if (degree == 0) zero_in_degree.push(vertex);
        }

        std::vector<T> result;
        while (!zero_in_degree.empty()) {
            T current = zero_in_degree.front();
            zero_in_degree.pop();
            result.push_back(current);

            for (const auto& edge : adj_list.at(current)) {
                in_degree[edge.destination]--;
                if (in_degree[edge.destination] == 0) {
                    zero_in_degree.push(edge.destination);
                }
            }
        }

        if (result.size() != adj_list.size()) {
            throw std::runtime_error("Graph contains a cycle! Topological sort not possible.");
        }

        return result;
    }

    // =========================================================================
    // Serialization & Debugging
    // =========================================================================

    std::string to_string() const {
        std::ostringstream ss;
        for (const auto& [vertex, edges] : adj_list) {
            ss << vertex << " -> ";
            for (size_t i = 0; i < edges.size(); ++i) {
                ss << edges[i].destination << "(" << edges[i].weight << ")";
                if (i + 1 < edges.size()) ss << ", ";
            }
            ss << "\n";
        }
        return ss.str();
    }
};

// =========================================================================
// Demonstrative Main Execution
// =========================================================================

int main() {
    std::cout << "--- 1. Directed Weighted Graph Setup ---\n";
    Graph<std::string> graph(true); // Directed graph

    graph.add_edge("A", "B", 4.0);
    graph.add_edge("A", "C", 2.0);
    graph.add_edge("B", "C", 1.0);
    graph.add_edge("B", "D", 5.0);
    graph.add_edge("C", "D", 8.0);
    graph.add_edge("C", "E", 10.0);
    graph.add_edge("D", "E", 2.0);

    std::cout << "Adjacency List:\n" << graph.to_string() << "\n";

    std::cout << "--- 2. Traversals (BFS & DFS) ---\n";
    std::cout << "BFS starting from 'A': ";
    graph.bfs("A", [](const std::string& node) { std::cout << node << " "; });
    std::cout << "\n";

    std::cout << "DFS starting from 'A': ";
    graph.dfs("A", [](const std::string& node) { std::cout << node << " "; });
    std::cout << "\n\n";

    std::cout << "--- 3. Shortest Path (Dijkstra's Algorithm) ---\n";
    auto [distances, predecessors] = graph.dijkstra("A");

    std::cout << "Shortest distances from 'A':\n";
    for (const auto& [node, dist] : distances) {
        std::cout << "  To " << node << ": " << dist << "\n";
    }

    std::string destination = "E";
    auto path = graph.reconstruct_path("A", destination, predecessors);
    std::cout << "\nShortest path from A to " << destination << ": ";
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i] << (i + 1 < path.size() ? " -> " : "");
    }
    std::cout << "\n\n";

    std::cout << "--- 4. Topological Sort (DAG) ---\n";
    Graph<std::string> dag(true);
    dag.add_edge("Build", "Test");
    dag.add_edge("Design", "Code");
    dag.add_edge("Code", "Build");
    dag.add_edge("Test", "Deploy");

    try {
        auto sorted_order = dag.topological_sort();
        std::cout << "Topological Order (Execution Order): ";
        for (size_t i = 0; i < sorted_order.size(); ++i) {
            std::cout << sorted_order[i] << (i + 1 < sorted_order.size() ? " -> " : "");
        }
        std::cout << "\n";
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }

    return 0;
}
