#ifndef GCALC_GRAPH_H
#define GCALC_GRAPH_H
#include "edge.h"
#include <exception>
#include <iostream>
#include <string>
#include <set>
#include <vector>

class Graph {
private:
    std::set<Node> nodes;
    std::set<Edge> edges;
    Graph(std::set<Node>&, std::set<Edge>&);
    Graph(std::set<Node>&&, std::set<Edge>&&);
    static bool validNode(const std::string&);
    void addEdge(const Edge&);
    void removeEdge(const Edge&);
public:
    Graph();
    Graph(const Graph&);
    Graph(Graph&&) noexcept;
    ~Graph() = default;

    Graph& operator=(const Graph&);
    void addNode(const Node&);
    void removeNode(const Node&);
    void addEdge(const Node&, const Node&);
    void removeEdge(const Node&, const Node&);
    Graph complement() const;
    friend std::ostream& operator<<(std::ostream&, const Graph&);
    static Graph unite(const Graph&, const Graph&);
    static Graph intersection(const Graph&, const Graph&);
    static Graph difference(const Graph&, const Graph&);
    static Graph product(const Graph&, const Graph&);

    class InvalidName : public std::invalid_argument {
        using std::invalid_argument::invalid_argument;
        using std::invalid_argument::what;
    };
    class NodeNotFound : public std::invalid_argument {
        using std::invalid_argument::invalid_argument;
        using std::invalid_argument::what;
    };
};

#endif //GCALC_GRAPH_H
