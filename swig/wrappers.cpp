#include "wrappers.h"
#include <iostream>

Graph* create() {return new Graph();}
void destroy(Graph* graph) {delete graph;}
Graph* addVertex(Graph* graph, const std::string& n) {
    try {
        graph->addNode(n);
    } catch(const Graph::GraphException& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    return graph;
}

Graph* addEdge(Graph* graph, const std::string& n1, const std::string& n2) {
    try {
        graph->addEdge(n1, n2);
    } catch(const Graph::GraphException& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    return graph;
}

void disp(Graph* graph) {std::cout << *graph << std::endl;}

Graph* graphUnion(Graph* g1, Graph* g2, Graph* out) {
    *out = Graph::unite(*g1, *g2);
    return out;
}

Graph* graphIntersection(Graph* g1, Graph* g2, Graph* out) {
    *out = Graph::intersection(*g1, *g2);
    return out;
}

Graph* graphDifference(Graph* g1, Graph* g2, Graph* out) {
    *out = Graph::difference(*g1, *g2);
    return out;
}

Graph* graphProduct(Graph* g1, Graph* g2, Graph* out) {
    *out = Graph::product(*g1, *g2);
    return out;
}

Graph* graphComplement(Graph* g, Graph* out) {
    *out = g->complement();
    return out;
}
