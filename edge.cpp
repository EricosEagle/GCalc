#include "edge.h"


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
