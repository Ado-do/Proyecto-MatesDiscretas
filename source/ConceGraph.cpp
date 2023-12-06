#include "ConceGraph.hpp"

#include <iostream>
#include <iomanip>
#include <cmath>

using std::string, std::cout, std::setw;

//* METODOS PUBLICOS *//
ConceGraph::ConceGraph() {
    string calleHorizontales[] = {"Los_Carrera","Maipu","Freire","Barros_Arana","O'Higgins","San_Martin","Cochrane","Chacabuco"};
    string calleVerticales[] = {"Prat","Serrano","Salas","Angol","Lincoyan","Rengo","Caupolican","Anibal_Pinto","Colo_Colo","Castellon","Tucapel","Orompello","Ongolmo","Paicavi"};

    // Ingresa vertices (intersecciones)
    for (size_t i = 0; i <= 7; i++) {
        for (size_t j = 0; j <= 13; j++) {
            string label = calleHorizontales[i] + "/" + calleVerticales[j];
            addVertex(j + (14 * i), label);
        }
    }

    // Ingresa aristas (calles)
    for (size_t i = 0; i <= 7; i++) {
        for (size_t j = 0; j <= 13; j++) {
            int current_id = j + (14 * i);

            // Caso general calles horizontales
            bool exceptions1 = (current_id == 49 || current_id == 50 || current_id == 97);
            bool rightToLeft = (i % 2 == 0 && j != 0);
            bool leftToRight = (i % 2 != 0 && j != 13) && !exceptions1;
            if (rightToLeft) {
                if (current_id == 97) { // Cambio de sentido en calle Cochrane
                    addEdge(current_id-1, current_id, 100);
                } else {
                    addEdge(current_id, (current_id)-1, 100);
                }
            }
            if (leftToRight) addEdge(current_id, (current_id)+1, 100);

            // Caso general calles verticales
            bool exceptions2 = (current_id == 21 || current_id == 35); // Pasos peatonales
            bool bottomToTop = (j % 2 == 0 && i != 0) ;
            bool topToBottom = (j % 2 != 0 && i != 7) && !exceptions2;
            if (bottomToTop) addEdge(current_id, (current_id)-14, 100);
            if (topToBottom) addEdge(current_id, (current_id)+14, 100);

            // Caso Avenidas (doble sentido)
            bool chacabuco = (i == 0 && j != 13);
            bool losCarrera = (i == 7 && j != 0);
            bool paicavi = (i != 0 && i <= 4 && j == 13);
            if (chacabuco) addEdge(current_id, (current_id)+1, 100);
            if (losCarrera) addEdge(current_id, (current_id)-1, 100);
            if (paicavi) addEdge(current_id, (current_id)-14, 100);

            // Caso Diagonal
            bool diagonal = (current_id == 81 || current_id == 96 || current_id == 111);
            int diagonalDist = 100 * sqrt(2);
            if (diagonal) {
                addEdge(current_id, (current_id)-15, diagonalDist);
                if (current_id != 111) {
                    addEdge(current_id, (current_id)+15, diagonalDist);
                }
            }
        }
    }
}

ConceGraph::~ConceGraph() {
    for (auto it = verticesMap.begin(); it != verticesMap.end(); it++) {
        Vertex* vertex = it->second;
        for (auto edge : vertex->edges) delete edge;
        delete vertex;
    }
}

void ConceGraph::print() {
    for (auto it = verticesMap.begin(); it != verticesMap.end(); it++) {
        Vertex* vertex = it->second;

        cout << vertex->label << setw(27 - vertex->label.length()) << " (" << vertex->id << ")" << " -> ";
        for (auto edge : vertex->edges) cout << edge->v2_id << " ";
        cout << std::endl;
    }
}

//* METODOS PRIVADOS *//
void ConceGraph::addVertex(int id, std::string label) {
    Vertex* vertex = new Vertex(id, label);
    verticesMap[id] = vertex;
}

void ConceGraph::addEdge(int v1_id, int v2_id, int dist) {
    Edge* edge = new Edge(v1_id, v2_id, dist);
    verticesMap[v1_id]->edges.push_back(edge);
}
