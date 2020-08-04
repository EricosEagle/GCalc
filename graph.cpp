#include "graph.h"
#include <algorithm>

template<class T>
static bool contains(const std::set<T>& set, const T& element) {
    // std::set doesn't have contains function in C++11
    return set.find(element) != set.end();
}

Graph::Graph(std::set<Node>& n, std::set<Edge>& e) : nodes(n), edges(e) {}
Graph::Graph(std::set<Node>&& n, std::set<Edge>&& e) : nodes(n), edges(e) {}

Graph::Graph() : Graph(std::set<Node>(), std::set<Edge>()) {}
Graph::Graph(const Graph& g) : Graph(std::set<Node>(g.nodes), std::set<Edge>(g.edges)) {}
Graph::Graph(Graph&& g) noexcept : Graph(g.nodes, g.edges) {}

Graph& Graph::operator=(const Graph& other) {
    if(this != &other) {
        std::copy(other.nodes.begin(), other.nodes.end(), nodes.begin());
        std::copy(other.edges.begin(), other.edges.end(), edges.begin());
    }
    return *this;
}

bool Graph::validNode(const std::string& name) {
    // TODO: Implement function
    return true;
}

void Graph::addNode(const Node& n) {
    if(!validNode(n)) {
        throw InvalidName("\"" + n + "\" is not a valid name for a node.");
    }
    nodes.insert(n);
}

void Graph::removeNode(const Node& n) {
    nodes.erase(n);
}

void Graph::addEdge(const Edge& e) {
    edges.insert(e);
}

void Graph::addEdge(const Node& src, const Node& dest) {
    if(!contains(nodes, src) || !contains(nodes, dest)) {
        throw NodeNotFound("One or more of the edge's nodes was not in the graph.");
    }
    addEdge(Edge(src, dest));
}

void Graph::removeEdge(const Edge& e) {
    edges.erase(e);
}

void Graph::removeEdge(const Node& src, const Node& dest) {
    if(contains(nodes, src) && contains(nodes, dest)) {
        removeEdge(Edge(src, dest));
    }
}

Graph Graph::unite(const Graph& g1, const Graph& g2) {
    Graph out;
    std::set_union(g1.nodes.begin(), g1.nodes.end(),
                   g2.nodes.begin(), g2.nodes.end(),
                   std::inserter(out.nodes, out.nodes.end()));
    std::set_union(g1.edges.begin(), g1.edges.end(),
                   g2.edges.begin(), g2.edges.end(),
                   std::inserter(out.edges, out.nodes.end()));
    return out;
}

Graph Graph::intersection(const Graph& g1, const Graph& g2) {
    Graph out;
    std::set_intersection(g1.nodes.begin(), g1.nodes.end(),
                          g2.nodes.begin(), g2.nodes.end(),
                          std::inserter(out.nodes, out.nodes.end()));
    std::set_intersection(g1.edges.begin(), g1.edges.end(),
                          g2.edges.begin(), g2.edges.end(),
                          std::inserter(out.edges, out.edges.end()));
    return out;
}

Graph Graph::difference(const Graph& g1, const Graph& g2) {
    Graph out;
    std::set_difference(g1.nodes.begin(), g1.nodes.end(),
                        g2.nodes.begin(), g2.nodes.end(),
                        std::inserter(out.nodes, out.nodes.end()));
    std::copy_if(g1.edges.begin(), g1.edges.end(),
                 std::inserter(out.edges, out.edges.end()),
                 [&](const Edge& e){contains(out.nodes, e.src) && contains(out.nodes, e.dest);});
    return out;
}

Graph Graph::product(const Graph& g1, const Graph& g2) {
    Graph out;
    // TODO: Implement Function
    return out;
}

Graph Graph::complement() const {
    Graph out;
    std::copy(nodes.begin(), nodes.end(), out.nodes.begin());
    for (auto i = nodes.begin(); i != nodes.end(); i++) {
        for (auto j = nodes.begin(); j != nodes.end(); j++) {
            if (i == j) {
                continue;
            }
            Edge e(*i, *j);
            if (!contains(edges, e)) {
                out.addEdge(e);
            }
        }
    }
    return out;
}

std::ostream& operator<<(std::ostream& os, const Graph& graph) {
    for(const Node& node : graph.nodes) {
        os << node << std::endl;
    }
    os << "$" << std::endl;
    for(const Edge& edge : graph.edges) {
        os << edge << std::endl;
    }
    return os;
}
