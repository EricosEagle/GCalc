%module wrappers

%{
#include "swig/wrappers.h"
#include "graph/graph.h"
%}

%include "std_string.i"

Graph* create();
void destroy(Graph*);
Graph* addVertex(Graph*, const std::string&);
Graph* addEdge(Graph*, const std::string&, const std::string&);
void disp(Graph*);
Graph* graphUnion(Graph*, Graph*, Graph*);
Graph* graphIntersection(Graph*, Graph*, Graph*);
Graph* graphDifference(Graph*, Graph*, Graph*);
Graph* graphProduct(Graph*, Graph*, Graph*);
Graph* graphComplement(Graph*, Graph*);
