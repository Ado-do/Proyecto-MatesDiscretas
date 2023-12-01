#pragma once;

#include <vector>
#include <string>
#include <map>

// Arco
struct Edge { 
    int node1; // Punte de salida
    int node2; // Punto de llegada
    int weight; // Distancia entre los dos puntos (metros)

    Edge(int node1, int node2, int weight) : node1(node1), node2(node2), weight(weight) {}
};

// Nodo
struct Node {
    std::string name;
    std::vector<Edge> edges;

    Node(std::string name) : name(name) {}
};

// Grafo
class Graph {
private:
    std::map<int, Node> nodes;

public:
    void addNode(int id, std::string name);
    void addEdge(int node1, int node2, int weight);

    void print();

    // Dijkstra
    std::vector<int> dijkstra(int start, int end);
};

/*
Common algorithms for graph traversal include:

Depth-First Search (DFS): This algorithm explores as far as possible along each branch before backtracking.

Breadth-First Search (BFS): This algorithm explores all the vertices at the present depth before going to the next level.

Dijkstra's Algorithm: This algorithm finds the shortest path from a starting node to all other nodes in the graph. It's a greedy algorithm that always chooses the node with the smallest distance.

Bellman-Ford Algorithm: This algorithm also finds the shortest path from a starting node to all other nodes. Unlike Dijkstra's, it can handle graphs with negative weight edges.

Floyd-Warshall Algorithm: This algorithm finds the shortest paths between all pairs of nodes. It's a dynamic programming algorithm.

A* Search Algorithm: This algorithm finds the shortest path from a starting node to a goal node. It uses a heuristic to estimate the cost to reach the goal, which makes it more efficient than Dijkstra's for many real-world problems.

Here are some suggestions for your next steps:

Implement the DFS and BFS algorithms for graph traversal.
Implement Dijkstra's algorithm for shortest path finding.
Test your implementations with some example graphs.
If your graph could have negative weight edges, consider implementing the Bellman-Ford algorithm.
If you need to find shortest paths between all pairs of nodes, consider implementing the Floyd-Warshall algorithm.
If you have a specific goal node and a good heuristic, consider implementing the A* search algorithm.
*/