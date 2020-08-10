#include "graph.h"
#include <algorithm>
#include <cctype>
#include <fstream>
using Edge = Graph::Edge;

template<class T>
static bool contains(const std::set<T>& set, const T& element) {
    // std::set doesn't have contains function in C++11
    return set.find(element) != set.cend();
}

Edge::Edge(const Node& s, const Node& d) : src(s), dest(d) {}

Edge::Edge(const Edge& edge) : Edge(edge.src, edge.dest) {}

std::string Edge::toString() const {
    return src + " " + dest;
}

std::ostream& operator<<(std::ostream& os, const Edge& edge) {
    os << edge.toString();
    return os;
}

bool operator<(const Edge& e1, const Edge& e2) {
    // Sort edges by src alphabetically, then by dest alphabetically
    return e1.src < e2.src || (e1.src == e2.src && e1.dest < e2.dest);
}

bool operator==(const Edge& e1, const Edge& e2) {
    return e1.src == e2.src && e1.dest == e2.dest;
}

Edge& Edge::operator=(const Edge& other) {
    if(this != &other) {
        src = other.src;
        dest = other.dest;
    }
    return *this;
}

Graph::GraphException::GraphException(const std::string& lit,
                                      const std::string& msg) : std::invalid_argument("'" + lit + "' " + msg) {}

Graph::InvalidName::InvalidName(const std::string& name) : GraphException(name, "is not a valid variable name.") {}
Graph::NodeNotFound::NodeNotFound(const std::string& node) : GraphException(node, "is not in the graph.") {}

Graph::Graph(std::set<Node>& n, std::set<Edge>& e) : nodes(n), edges(e) {}
Graph::Graph(std::set<Node>&& n, std::set<Edge>&& e) : nodes(n), edges(e) {}

Graph::Graph() : Graph(std::set<Node>(), std::set<Edge>()) {}
Graph::Graph(const Graph& g) : Graph(std::set<Node>(g.nodes), std::set<Edge>(g.edges)) {}
Graph::Graph(Graph&& g) noexcept : Graph(g.nodes, g.edges) {}

Graph& Graph::operator=(const Graph& other) {
    if(this != &other) {
        nodes.clear();
        edges.clear();
        nodes.insert(other.nodes.begin(), other.nodes.end());
        edges.insert(other.edges.begin(), other.edges.end());
    }
    return *this;
}

bool Graph::validNode(const std::string& name) {
    bool isValid = true;
    int bracketCounter = 0;
    for(int i = 0; isValid && i < (int) name.size(); i++) {
        char c = name[i];
        switch(c) {
            case '[':
                bracketCounter++;
                break;
            case ']':
                bracketCounter--;
                isValid = bracketCounter >= 0;
                break;
            case ';':
                isValid = bracketCounter > 0;
                break;
            default:
                isValid = isalnum(c);
        }
    }
    return isValid && bracketCounter == 0;
}

void Graph::addNode(const Node& n) {
    if(!validNode(n)) {
        throw InvalidName(n);
    }
    nodes.insert(n);
}

void Graph::removeNode(const Node& n) {
    nodes.erase(n);
}

bool Graph::containsNode(const Node& n) const {
    return contains(nodes, n);
}

bool Graph::adjacent(const Node& n1, const Node& n2) const {
    if(!contains(nodes, n1)) {
        throw NodeNotFound(n1);
    } else if(!contains(nodes, n2)) {
        throw NodeNotFound(n2);
    }
    return contains(edges, Edge(n1, n2));
}

std::set<Node> Graph::neighbours(const Node& n) const {
    if(!contains(nodes, n)) {
        throw NodeNotFound(n);
    }
    std::set<Node> out;
    for(const Edge& e : edges) {
        if(e.src == n) {
            out.insert(e.dest);
        }
    }
    return out;
}

void Graph::clearEdges() {
    edges.clear();
}

void Graph::clearAll() {
    nodes.clear();
    clearEdges();
}

const std::set<Node>& Graph::getNodes() const {
    return nodes;
}

static inline void binaryWriteUint(unsigned int num, std::ofstream& os) {
    os.write((const char*) &num, sizeof(unsigned int));
}

static void binaryWriteStr(const std::string& str, std::ofstream& os) {
    binaryWriteUint(str.size(), os);
    os.write(str.c_str(), str.size());
}

void Graph::save(const std::string& fname) const {
    std::ofstream graphFile(fname, std::ios::binary);
    binaryWriteUint(nodes.size(), graphFile);
    binaryWriteUint(edges.size(), graphFile);
    for(const Node& n : nodes) {
        binaryWriteStr(n, graphFile);
    }
    for(const Edge& e : edges) {
        binaryWriteStr(e.src, graphFile);
        binaryWriteStr(e.dest, graphFile);
    }
    graphFile.close();
}

static unsigned int binaryReadUint(std::ifstream& is) {
    unsigned int out;
    is.read((char*) &out, sizeof(unsigned int));
    return out;
}

static std::string binaryReadStr(std::ifstream& is) {
    unsigned int len = binaryReadUint(is);
    std::string result;
    result.resize(len);
    is.read(&result[0], len);
    return result;
}

Graph Graph::load(const std::string& fname) {
    std::ifstream graphFile(fname, std::ios::binary);
    Graph result;
    unsigned int vertexNum = binaryReadUint(graphFile), edgeNum = binaryReadUint(graphFile);
    for(unsigned int i = 0; i < vertexNum; i++) {
        result.addNode(binaryReadStr(graphFile));
    }
    for(unsigned int i = 0; i < edgeNum; i++) {
        result.addEdge(binaryReadStr(graphFile), binaryReadStr(graphFile));
    }
    graphFile.close();
    return result;
}

void Graph::addEdge(const Edge& e) {
    if(e.src == e.dest) {
        throw Graph::Edge::EdgeError("A node cannot be connected to itself.");
    } else if(!contains(nodes, e.src)) {
        throw NodeNotFound(e.src);
    } else if(!contains(nodes, e.dest)) {
        throw NodeNotFound(e.dest);
    }
    edges.insert(e);
}

void Graph::addEdge(const Node& src, const Node& dest) {
    addEdge(Edge(src, dest));
}

void Graph::removeEdge(const Edge& e) {
    edges.erase(e);
}

void Graph::removeEdge(const Node& src, const Node& dest) {
    removeEdge(Edge(src, dest));
}

Graph Graph::unite(const Graph& g1, const Graph& g2) {
    Graph out;
    std::set_union(g1.nodes.begin(), g1.nodes.end(),
                   g2.nodes.begin(), g2.nodes.end(),
                   std::inserter(out.nodes, out.nodes.end()));
    std::set_union(g1.edges.begin(), g1.edges.end(),
                   g2.edges.begin(), g2.edges.end(),
                   std::inserter(out.edges, out.edges.end()));
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
                 [&](const Edge& e){return contains(out.nodes, e.src) && contains(out.nodes, e.dest);});
    return out;
}

Node Graph::nodeProduct(const Node& n1, const Node& n2) {
    return "[" + n1 + ";" + n2 + "]";
}

Graph Graph::product(const Graph& g1, const Graph& g2) {
    Graph out;
    for(const Node& n1 : g1.nodes) {
        for(const Node& n2 : g2.nodes) {
            out.addNode(nodeProduct(n1, n2));
        }
    }
    for(const Edge& e1: g1.edges) {
        for(const Edge& e2: g2.edges) {
            out.addEdge(Edge(nodeProduct(e1.src, e2.src), nodeProduct(e1.dest, e2.dest)));
        }
    }
    return out;
}

Graph Graph::complement() const {
    Graph out;
    out.nodes.insert(nodes.begin(), nodes.end());
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
    os << "$";
    for(const Edge& edge : graph.edges) {
        os << std::endl << edge;
    }
    return os;
}
