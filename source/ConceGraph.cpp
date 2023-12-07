#include "ConceGraph.hpp"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <queue>
#include <array>
#include <algorithm>

using std::string, std::cout, std::endl, std::setw, std::vector, std::pair, std::array;

//* METODOS PUBLICOS *//
ConceGraph::ConceGraph() {
    array<string, 8> horizontales = {"Av. los Carrera","Maipu","Freire","Barros Arana","O'Higgins","San Martin","Cochrane","Av. Chacabuco"};
    array<string, 14> verticales = {"Arturo Prat","Serrano","Obispo Hipolito Salas","Angol","Lincoyan","Rengo","Caupolican","Anibal Pinto","Colo Colo","Castellon","Tucapel","Orompello","Ongolmo","Paicavi"};

    int it = 0, contDiagonal = 0;
    for (size_t i = 0; i < horizontales.size(); i++) {
        for (size_t j = 0; j < verticales.size(); j++, it++) {
            //* Agregar vertices
            addVertex(it, horizontales[i] + "/" + verticales[j]);

            //* Agregar aristas
            // Caso general calles horizontales (0 -> 1300)
            bool horizontalPar = (i % 2 == 0);

            bool rightToLeft = (horizontalPar && j != 0);
            if (rightToLeft) {
                size_t v1 = (it == 97) ? it-1 : it;
                size_t v2 = (it == 97) ? it : it-1;
                int r1 = 1 + (100 * (j-1)), r2 = r1 + 99;
                addEdge(horizontales[i], v1, v2, r1, r2, 100);
            }

            bool peatonal1 = (it == 49 || it == 50); // Pasos peatonales
            bool leftToRight = (!horizontalPar && j != 13) && !peatonal1;
            if (leftToRight) {
                int r1 = 1 + (100 * j), r2 = r1 + 99;
                addEdge(horizontales[i], it, it+1, r1, r2, 100);
            }

            // Caso general calles verticales (800 -> 100)
            bool verticalPar = (j % 2 == 0);

            bool bottomToTop = (verticalPar && i != 0) ;
            if (bottomToTop) {
                int r1 = 800 - (100 * i), r2 = r1 - 99;
                addEdge(verticales[j], it, it-14, r1, r2, 100);
            }

            bool peatonal2 = (it == 21 || it == 35); // Pasos peatonales
            bool topToBottom = (!verticalPar && i != 7) && !peatonal2;
            if (topToBottom) {
                int r1 = 800 - (100 * (i-1)), r2 = r1 - 99;
                addEdge(verticales[j], it, it+14, r1, r2, 100);
            }

            // Caso Diagonal (1300 -> 1100)
            bool diagonal = (it == 81 || it == 96 || it == 111);
            int diagonalDist = 100 * sqrt(2);
            if (diagonal) {
                addEdge("Diagonal", it, it-15, 1001 + (100 * contDiagonal), 1100 + (100 * contDiagonal), diagonalDist);
                if (it == 66) addEdge("Diagonal", it+15, it, 1001 + (100 * contDiagonal), 1100 + (100 * contDiagonal), diagonalDist);
                contDiagonal++;
            }

            // Casos doble sentido
            bool losCarrera = (i == 0 && j != 13);
            bool chacabuco = (i == 7 && j != 0);
            bool paicavi = (i != 0 && i <= 4 && j == 13);
            if (losCarrera) {
                addEdge(horizontales[i], it, it+1, 1 + (100 * j), 100 + (100 * j), 100);
            }
            if (chacabuco) {
                addEdge(horizontales[i], it, it-1, 1 + (100 * (j-1)), 100 + (100 * (j-1)), 100);
            }
            if (paicavi) {
                addEdge(verticales[j], it, it-14, 800 - (100 * (i-1)), 701 - (100 * (i-1)), 100);
            }
            if (diagonal && it != 111) {
                addEdge("Diagonal", it, it+15, 1001 + (100 * contDiagonal), 1100 + (100 * contDiagonal), diagonalDist);
            }
        }
    }
}

ConceGraph::~ConceGraph() {
    for (auto vertex : vertices) {
        for (auto edge : vertex->getEdges()) 
            delete edge;
        delete vertex;
    }
}

// Camino mas corto entre dos intersecciones, pasando por una posible tercera
vector<string> ConceGraph::getShortestPath(const string& start, const string& end, const string& third) {
    vector<string> path;
    path.push_back(start);
    
    vector<string> dijkstraPath;
    if (third != "") {
        cout << "* Buscando camino mas corto entre " << start << " y " << end << ", pasando por " << third << ":" << endl; // DEBUG
        size_t v1 = findStartByAddress(start), v2 = findStartByAddress(third), v3 = findEndByAddress(end);
        if (v1 == INT_MAX || v2 == INT_MAX || v3 == INT_MAX) {
            path.push_back("No se encontro camino");
            return path;
        }
        dijkstraPath = dijkstra(findStartByAddress(start), findStartByAddress(third));
        path.insert(path.end(), dijkstraPath.begin(), dijkstraPath.end());
        path.push_back(third);
        dijkstraPath = dijkstra(findStartByAddress(third), findEndByAddress(end));
        path.insert(path.end(), dijkstraPath.begin(), dijkstraPath.end());
    } else {
        cout << "* Buscando camino mas corto entre " << start << " y " << end << ":" << endl; // DEBUG
        size_t v1 = findStartByAddress(start), v2 = findEndByAddress(end);
        if (v1 == INT_MAX || v2 == INT_MAX) {
            path.push_back("No se encontro camino");
            return path;
        }
        dijkstraPath = dijkstra(v1, v2);
        path.insert(path.end(), dijkstraPath.begin(), dijkstraPath.end());
        path.push_back(end);
    }
    cout << endl; // DEBUG

    return path;
}

void ConceGraph::print() {
    cout << "* Grafo del centro de Concepcion:" << endl;
    for (auto vertex : vertices) {
        string l = vertex->getLabel();
        cout << l << setw(40 - l.length()) << " (" << vertex->getId() << ") -> ";

        for (auto edge : vertex->getEdges()) {
            auto r = edge->getRange();
            cout << edge->getLabel() << '[' << r.first << ',' << r.second << ']' << " ";
            // cout << edge->getDestination() << " ";
        }
        cout << endl;
    }
    cout << endl;
}

//* METODOS PRIVADOS *//
void ConceGraph::addVertex(size_t id, const string& label) {
    Vertex* vertex = new Vertex(id, label);
    vertices.push_back(vertex);
}

void ConceGraph::addEdge(const string& label, size_t v1, size_t v2, int r1, int r2, int weight) {
    Edge* edge = new Edge(label, v1, v2, r1, r2, weight);
    vertices[v1]->addEdge(edge);
}

// ? Caso doble sentidoXD
size_t ConceGraph::findStartByAddress(const string& address) {
    cout << "Buscando punto de partida desde " << address << endl; //! DEBUG
    int number;
    for (size_t i = 0; i < address.length(); i++) {
        if (isdigit(address[i])) {
            number = stoi(address.substr(i, address.length()));
            break;
        }
    }
    
    for (auto vertex : vertices) {
        string intersection = vertex->getLabel();
        if (isSubStr(intersection, address)) {
            for (auto edge : vertex->getEdges()) {
                string road = edge->getLabel();
                if (isSubStr(road, address)) {
                    auto range = edge->getRange();
                    int minor = (range.first < range.second) ? range.first : range.second;
                    int major = (range.first > range.second) ? range.first : range.second;
                    if (number >= minor && number <= major) {
                        cout << "Encontrado punto de partida: " << intersection << " (" << vertex->getId() << ")" << endl; // DEBUG
                        return edge->getSource();
                    }
                }
            }
        }
    }

    cout << "No se encontro punto de partida" << endl; // DEBUG
    return INT_MAX;
}

size_t ConceGraph::findEndByAddress(const string& address) {
    cout << "Buscando punto de llegada desde " << address << endl; //! DEBUG
    int number;
    for (size_t i = 0; i < address.length(); i++) {
        if (isdigit(address[i])) {
            number = stoi(address.substr(i, address.length()));
            break;
        }
    }
    
    for (auto vertex : vertices) {
        string intersection = vertex->getLabel();
        if (isSubStr(intersection, address)) {
            for (auto edge : vertex->getEdges()) {
                string road = edge->getLabel();
                if (isSubStr(road, address)) {
                    auto range = edge->getRange();
                    int minor = (range.first < range.second) ? range.first : range.second;
                    int major = (range.first > range.second) ? range.first : range.second;
                    if (number >= minor && number <= major) {
                        cout << "Encontrado punto de llegada: " << intersection << " (" << vertex->getId() << ")" << endl; // DEBUG
                        return edge->getDestination();
                    }
                }
            }
        }
    }

    cout << "No se encontro punto de llegada" << endl; // DEBUG
    return INT_MAX;
}

bool ConceGraph::isSubStr(const string& str1, const string& str2) {
    bool isSubStr = (modStr(str1).find(modStr(str2)) != string::npos);
    return isSubStr;
}

vector<string> ConceGraph::dijkstra(size_t start, size_t end) {
    vector<int> distances(vertices.size(), INT_MAX);
    vector<size_t> previous(vertices.size(), -1);
    std::priority_queue<pair<int, size_t>, vector<pair<int, size_t>>, std::greater<pair<int, size_t>>> pq;

    distances[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        size_t current = pq.top().second;
        pq.pop();

        for (auto edge : vertices[current]->getEdges()) {
            size_t v = edge->getDestination();
            int dist = edge->getWeight();

            if (distances[v] > distances[current] + dist) {
                distances[v] = distances[current] + dist;
                previous[v] = current;
                pq.push({distances[v], v});
            }
        }
    }

    vector<string> path;
    for (size_t i = end; i != size_t(-1); i = previous[i]) {
        path.push_back(vertices[i]->getLabel());
    }
    std::reverse(path.begin(), path.end());

    // cout << "Distancia total: " << distances[end] << " metros" << endl;
    // for (size_t i = 0; i < path.size(); i++) {
    //     cout << path[i];
    //     if (i != path.size()-1) cout << " -> ";
    // }

    return path;
}

// Modificar string ()
string ConceGraph::modStr(const string& str) {
    vector<char> chars;
    for (char c : str) {
        if (isalpha(c)) chars.push_back(towlower(c));
    }
    string newStr(chars.begin(), chars.end());
    return newStr;
}
