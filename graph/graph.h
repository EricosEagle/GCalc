#ifndef GCALC_GRAPH_H
#define GCALC_GRAPH_H
#include <exception>
#include <iostream>
#include <string>
#include <set>
#include <vector>

typedef std::string Node;

class Graph {
public:

    class GraphException : public std::invalid_argument {
    public:
        GraphException(const std::string&, const std::string&);
        using std::invalid_argument::what;
    };

    class InvalidName : public GraphException {
    public:
        explicit InvalidName(const std::string&);
        using GraphException::what;
    };

    class NodeNotFound : public GraphException {
    public:
        explicit NodeNotFound(const std::string&);
        using GraphException::what;
    };

    struct Edge {
        Node src, dest;
        Edge(const Node&, const Node&);
        Edge(const Edge&);
        ~Edge() = default;
        std::string toString() const;
        friend bool operator<(const Edge&, const Edge&);
        friend bool operator==(const Edge&, const Edge&);
        Edge& operator=(const Edge&);

        class EdgeError : public std::invalid_argument {
        public:
            using std::invalid_argument::invalid_argument;
            using std::invalid_argument::what;
        };
    };

private:
    std::set<Node> nodes;
    std::set<Edge> edges;
    Graph(std::set<Node>&, std::set<Edge>&);
    Graph(std::set<Node>&&, std::set<Edge>&&);
    static Node nodeProduct(const Node&, const Node&);

public:
    Graph();
    Graph(const Graph&);
    Graph(Graph&&) noexcept;
    ~Graph() = default;

    Graph& operator=(const Graph&);
    static bool validNode(const std::string&);
    void addNode(const Node&);
    void clearEdges();
    void clearAll();
    void removeNode(const Node&);
    bool containsNode(const Node&) const;
    bool adjacent(const Node&, const Node&) const;
    std::set<Node> neighbours(const Node&) const;
    const std::set<Node>& getNodes() const;
    void save(const std::string& fname) const;
    static Graph load(const std::string& fname);
    void addEdge(const Edge&);
    void removeEdge(const Edge&);
    void addEdge(const Node&, const Node&);
    void removeEdge(const Node&, const Node&);
    Graph complement() const;
    friend std::ostream& operator<<(std::ostream&, const Graph&);
    static Graph unite(const Graph&, const Graph&);
    static Graph intersection(const Graph&, const Graph&);
    static Graph difference(const Graph&, const Graph&);
    static Graph product(const Graph&, const Graph&);
};

std::ostream& operator<<(std::ostream&, const Graph::Edge&);

#endif //GCALC_GRAPH_H
