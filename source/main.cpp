#include "ConceGraph.hpp"

#include <iostream>

using std::string, std::cout, std::endl, std::vector;

/*
    Notas:
    - Los nodos se identifican con el nombre de las intersecciones de las calles (Rengo-Cochrane)
    - Los arcos se identifican con el numero de las calles (Rengo 100)
    - Los pesos de los arcos se identifican con la distancia entre las intersecciones de las calles (100 metros)
    - Los nodos se guardan en un mapa, donde la llave es el nombre de la interseccion y el valor es el nodo
*/

// Compilar con: g++ -Wall -o main.exe source/main.cpp source/ConceGraph.cpp

// ./main.exe "Av. Los Carrera 200" "Salas 655"

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
    string start = "Los Carrera 200", end = "Salas 655";
    vector<string> path = graph.getShortestPath(start, end);
    printPath(path);

    // if (argc == 3) {
    //     string start = argv[1], end = argv[2];

    //     vector<string> path = graph.getShortestPath(start, end);

    //     cout << "Camino mas corto entre " << start << " y " << end << ": ";
    //     printPath(path);

    // } else if (argc == 4) {
    //     string start = argv[1], end = argv[2], third = argv[3];

    //     vector<string> path = graph.getShortestPath(start, end, third);

    //     cout << "Camino mas corto entre " << start << " y " << end << ", pasando por " << third << ": ";
    //     printPath(path);
        
    // } else {
    //     cout << "Error: Numero de argumentos invalido" << endl;
    // }

    return 0;
}
