#include "Graph.hpp"

//* METODOS PUBLICOS *//
ConceGraph::ConceGraph() {
    // Ingresa los vertices y aristas del grafo de Concepcion
}

ConceGraph::~ConceGraph() {
    // Liberar memoria de los vertices y aristas
}

//* METODOS PRIVADOS *//
void ConceGraph::print() {

}
void ConceGraph::addVertex(int id, std::string label) {
    verticesMap[id] = new Vertex(id, label);
}

void ConceGraph::addEdge(int v1_id, int v2_id, int dist) {
    Edge* edge = new Edge(v1_id, v2_id, dist);
    verticesMap[v1_id]->incidentEdges.push_back(edge);
    verticesMap[v2_id]->incidentEdges.push_back(edge);
}
