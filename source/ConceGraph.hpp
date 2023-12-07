#pragma once

#include <vector>
#include <string>
#include <map>


//* Arista (Calle)
class Edge {
    std::string label; // Nombre de la calle (Rengo)
    std::pair<int, int> ids; // Intersecciones de la calle (Rengo/Cochrane)
    std::pair<int, int> range; // Range de la calle (Rengo 100 - 200)
    int weight; // Distancia entre intersecciones
public:
    Edge(std::string label, size_t v1, size_t v2, int r1, int r2, int weight)
        : label(label), ids(std::make_pair(v1, v2)), range(std::make_pair(r1, r2)), weight(weight) {}

    std::string getLabel() const { return label; }
    size_t getSource() const { return ids.first; }
    size_t getDestination() const { return ids.second; }
    int getWeight() const { return weight; }
    std::pair<int, int> getRange() const { return range; }
};

//* Vertice (Intersección de calles)
class Vertex {
    size_t id; // Indice del vertice
    std::string label; // Nombre de la intersección (Rengo/Cochrane)
    std::vector<Edge*> edges;
public:
    Vertex(size_t id, std::string label) : id(id), label(label) {}

    std::string getLabel() const { return label; }
    std::vector<Edge*> getEdges() const { return edges; }
    void addEdge(Edge* edge) { edges.push_back(edge); }
    size_t getId() const { return id; }
};

//* Grafo (Mapa de Concepción)
class ConceGraph {
    std::vector<Vertex*> vertices;
    
    void addVertex(size_t id, const std::string& label);
    void addEdge(const std::string& label, size_t v1, size_t v2, int r1, int r2, int weight);
    size_t findStartByAddress(const std::string& label);
    size_t findEndByAddress(const std::string& label);
    std::vector<std::string> dijkstra(size_t start, size_t end);
    std::string modStr(const std::string& str);
    bool isSubStr(const std::string& str1, const std::string& str2);

public:
    ConceGraph();
    ~ConceGraph();
    
    std::vector<std::string> getShortestPath(const std::string& start, const std::string& end, const std::string& third = "");
    void print();
};
