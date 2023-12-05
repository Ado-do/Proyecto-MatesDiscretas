#pragma once;

#include <vector>
#include <string>
#include <map>

//* Arista (Calle)
struct Edge {
    int v1_id, v2_id; // Identificadores de los vertices (intersecciones) que conecta la arista (calle)
    int distance; // Distancia entre los dos puntos (metros)
    Edge(int v1, int v2, int dist) : v1_id(v1), v2_id(v2), distance(dist) {}
};

//* Vertice (Interseccion de calles)
struct Vertex {
    int id; // Identificador del vertice (interseccion)
    std::string label; // Etiqueta del vertice (nombre de la interseccion)
    std::vector<Edge*> incidentEdges; // Aristas incidentes al vertice (calles que conectan a la interseccion)
    Vertex(int id, std::string label) : id(id), label(label) {}
};

//* Grafo
class ConceGraph {
    std::map<int, Vertex*> verticesMap; // Contenedor de vertices (mapa de intersecciones)
    // std::vector<Edge*> edgesList; // Vector de aristas (calles)

    void addVertex(int id, std::string label);
    void addEdge(int v1_id, int v2_id, int dist);

public:
    ConceGraph();
    ~ConceGraph();

    void print();
    std::vector<int> getShortestPath(std::string start, std::string end); // Dijkstra
};
