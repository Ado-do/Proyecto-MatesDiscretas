#include "ConceGraph.hpp"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <queue>
#include <algorithm>
#include <climits>

using std::cout;
using std::endl;
using std::setw;
using std::string;
using std::vector;
using std::pair;

ConceGraph::ConceGraph() {
    int vertexIndex = 0;
    for (size_t i = 0; i < horizontales.size(); i++) {
        for (size_t j = 0; j < verticales.size(); j++) {
            addVertices(i, j, vertexIndex);
            addEdges(i, j, vertexIndex);
            vertexIndex++;
        }
    }
}

void ConceGraph::addVertices(size_t i, size_t j, int vertexIndex) {
    string intersection = horizontales[i] + "/" + verticales[j];

    int v = vertexIndex;
    bool isDiagonal = (v == 66 || v == 81 || v == 96 || v == 111);
    if (isDiagonal) intersection += "/Diagonal";

    addVertex(vertexIndex, intersection);
}

void ConceGraph::addEdges(size_t i, size_t j, int vertexIndex) {
    bool isEvenHorizontal = (i % 2 == 0);
    bool isEvenVertical = (j % 2 == 0);
    bool isDiagonal = (vertexIndex == 81 || vertexIndex == 96 || vertexIndex == 111);
    bool isHorizontalPedestrian = (vertexIndex == 49 || vertexIndex == 50);
    bool isVerticalPedestrian = (vertexIndex == 21 || vertexIndex == 35);

    // Casos generales
    if (isEvenHorizontal) {
        addEdgeForEvenHorizontal(vertexIndex, horizontales[i], j);
    }
    if (!isEvenHorizontal && !isHorizontalPedestrian) {
        addEdgeForOddHorizontal(vertexIndex, horizontales[i], j);
    }

    if (isEvenVertical) {
        addEdgeForEvenVertical(vertexIndex, verticales[j], i);
    }
    if (!isEvenVertical && !isVerticalPedestrian) {
        addEdgeForOddVertical(vertexIndex, verticales[j], i);
    }

    if (isDiagonal) {
        int diagonalDist = 100 * sqrt(2);
        int r = 1001 + (100 * (i-5));
        addEdge("Diagonal", vertexIndex, vertexIndex-15, r, r+99, diagonalDist);
    }

    // Casos doble sentido
    bool losCarrera = (i == 0 && j != 13);
    bool chacabuco = (i == 7 && j != 0);
    bool paicavi = (i != 0 && i <= 4 && j == 13);
    if (losCarrera) {
        int r = 1 + (100 * j);
        addEdge(horizontales[i], vertexIndex, vertexIndex+1, r, r+99, 100);
    }
    if (chacabuco) {
        int r = 1 + (100 * (j-1));
        addEdge(horizontales[i], vertexIndex, vertexIndex-1, r, r+99, 100);
    }
    if (paicavi) {
        int r = 800 - (100 * (i-1));
        addEdge(verticales[j], vertexIndex, vertexIndex-14, r, r-99, 100);
    }
    if (isDiagonal && vertexIndex != 111) {
        int diagonalDist = 100 * sqrt(2);
        int r = 1101 + (100 * (i-5));
        addEdge("Diagonal", vertexIndex, vertexIndex+15, r, r+99, diagonalDist);
    }
}

void ConceGraph::addVertex(size_t id, const string& label) {
    Vertex* vertex = new Vertex(id, label);
    vertices.push_back(vertex);
}

void ConceGraph::addEdge(const string& label, size_t v1, size_t v2, int r1, int r2, int weight) {
    Edge* edge = new Edge(label, v1, v2, r1, r2, weight);
    vertices[v1]->addEdge(edge);
}

void ConceGraph::print() {
    cout << "* Grafo del centro de Concepcion:" << endl;
    for (auto vertex : vertices) {
        string l = vertex->getLabel();
        cout << l << setw(40 - l.length()) << " (" << vertex->getId() << ") -> ";
        for (auto edge : vertex->getEdges()) {
            auto r = edge->getRange();
            cout << edge->getDestination() << '[' << r.first << ',' << r.second << ']' << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// Camino mas corto entre dos intersecciones, pasando por una posible tercera
vector<string> ConceGraph::getShortestPath(const string& start, const string& end, const string& third) {
    vector<string> path;
    path.push_back(start);
    
    vector<string> dijkstraPath;
    if (third != "") {
        cout << "* Buscando camino mas corto entre " << start << " y " << end << ", pasando por " << third << ":" << endl; //! DEBUG
        size_t v1 = findStartByAddress(start), v2 = findStartByAddress(third), v3 = findEndByAddress(end);
        if (v1 == INT_MAX || v2 == INT_MAX || v3 == INT_MAX) {
            path.push_back("No se encontro camino");
            return path;
        }
        dijkstraPath = dijkstra(v1, v2);
        path.insert(path.end(), dijkstraPath.begin(), dijkstraPath.end());
        path.push_back(third);
        dijkstraPath = dijkstra(v2, v3);
        path.insert(path.end(), dijkstraPath.begin(), dijkstraPath.end());
        path.push_back(end);

    } else {
        cout << "* Buscando camino mas corto entre " << start << " y " << end << ":" << endl; //! DEBUG
        size_t v1 = findStartByAddress(start), v2 = findEndByAddress(end);
        if (v1 == INT_MAX || v2 == INT_MAX) {
            path.push_back("No se encontro camino");
            return path;
        }
        dijkstraPath = dijkstra(v1, v2);
        path.insert(path.end(), dijkstraPath.begin(), dijkstraPath.end());
        path.push_back(end);
    }
    cout << endl; //! DEBUG

    return path;
}


// ? Caso doble sentidoXD
size_t ConceGraph::findStartByAddress(const string& address) {
    cout << "Buscando punto de partida desde " << address << endl; //! DEBUG
    int number = -1;
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
                        return edge->getDestination();
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
    int number = -1;
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
                        return edge->getSource();
                    }
                }
            }
        }
    }

    cout << "No se encontro punto de llegada" << endl; // DEBUG
    return INT_MAX;
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
    for (int i = end; i != -1; i = previous[i]) {
        // path.push_back(vertices[i]->getLabel());
        path.push_back(std::to_string(i));
    }
    std::reverse(path.begin(), path.end());

    // cout << "Distancia total aprox.: " << distances[end] << " metros" << endl;
    // for (size_t i = 0; i < path.size(); i++) {
        // cout << path[i];
        // if (i != path.size()-1) cout << " -> ";
    // }

    return path;
}

void ConceGraph::addEdgeForEvenHorizontal(size_t vertexIndex, const string& streetName, size_t j) {
    if (j != 0) {
        int v1 = (vertexIndex == 97) ? vertexIndex-1 : vertexIndex;
        int v2 = (vertexIndex == 97) ? vertexIndex : vertexIndex-1;
        int r = 1 + (100 * (j-1));
        addEdge(streetName, v1, v2, r, r+99, 100);
    }
}
void ConceGraph::addEdgeForOddHorizontal(size_t vertexIndex, const string& streetName, size_t j) {
    if (j != 13) {
        int r = 1 + (100 * j);
        addEdge(streetName, vertexIndex, vertexIndex+1, r, r+99, 100);
    }
}
void ConceGraph::addEdgeForEvenVertical(size_t vertexIndex, const string& streetName, size_t i) {
    if (i != 0) {
        int r = 800 - (100 * (i-1));
        addEdge(streetName, vertexIndex, vertexIndex-14, r, r-99, 100);
    }
}
void ConceGraph::addEdgeForOddVertical(size_t vertexIndex, const string& streetName, size_t i) {
    if (i != 7) {
        int r = 800 - (100 * i);
        addEdge(streetName, vertexIndex, vertexIndex+14, r, r-99, 100);
    }
}
ConceGraph::~ConceGraph() {
    for (auto vertex : vertices) {
        for (auto edge : vertex->getEdges()) 
            delete edge;
        delete vertex;
    }
}

// Modificar string a un formato mas facil de comparar
string ConceGraph::modStr(const string& str) {
    vector<char> chars;
    for (char c : str) {
        if (isalpha(c)) chars.push_back(towlower(c));
    }
    string newStr(chars.begin(), chars.end());
    return newStr;
}

// Verificar si un string es substring de otro
bool ConceGraph::isSubStr(const string& str1, const string& str2) {
    bool isSubStr = (modStr(str1).find(modStr(str2)) != string::npos);
    return isSubStr;
}