#ifndef GCALC_EDGE_H
#define GCALC_EDGE_H
#include <iostream>
#include <string>
#define Node std::string

struct Edge {
    const Node& src;
    const Node& dest;
    Edge(const Node&, const Node&);
    Edge(const Edge&);
    ~Edge() = default;
    std::string toString() const;
    friend bool operator<(const Edge&, const Edge&);
    friend bool operator==(const Edge&, const Edge&);
    Edge& operator=(const Edge&) = delete;
};

std::ostream& operator<<(std::ostream&, const Edge&);

#endif //GCALC_EDGE_H
