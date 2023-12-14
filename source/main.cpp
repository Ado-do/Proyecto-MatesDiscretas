#include "ConceGraph.hpp"

#include <iostream>

using std::vector;
using std::string;
using std::cout;
using std::endl;

//* Compilar con: g++ -Wall -o concegraph.exe source/main.cpp source/ConceGraph.cpp

//* ./concegraph "castellon 298" "diagonal 1037" "freire 45"

void printPath(vector<string> path) {
    cout << "Camino: ";
    for (size_t i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i != path.size()-1) cout << " -> ";
    }
    cout << endl;
}

int main(int argc, char *argv[]) {
    ConceGraph graph;

    // Imprimir grafo
    graph.print();

    // Camino mas corto entre dos o tres intersecciones
    vector<string> path;
    
    if (argc == 3) {
        string start = argv[1], end = argv[2];
        path = graph.getShortestPath(start, end);
        printPath(path);
    } else if (argc == 4) {
        string start = argv[1], end = argv[2], third = argv[3];
        path = graph.getShortestPath(start, end, third);
        printPath(path);
    } else {
        cout << "Error: Numero de argumentos invalido" << endl;
    }

    return 0;
}
