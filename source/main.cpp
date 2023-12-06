#include "ConceGraph.hpp"

#include <iostream>

/*
    Notas:
    - Los nodos se identifican con el nombre de las intersecciones de las calles (Rengo-Cochrane)
    - Los arcos se identifican con el numero de las calles (Rengo 100)
    - Los pesos de los arcos se identifican con la distancia entre las intersecciones de las calles (100 metros)
    - Los nodos se guardan en un mapa, donde la llave es el nombre de la interseccion y el valor es el nodo
*/

// Compilar: g++ -std=c++11 -o main source/main.cpp source/ConceGraph.cpp

int main(int argc, char *argv[]) {
    ConceGraph graph;

    // Imprimir grafo
    graph.print();

    // Imprimir linea de argumentos
    // std::cout << "Argumentos: ";
    // for (int i = 0; i < argc; i++) std::cout << argv[i] << " ";

    return 0;
}
