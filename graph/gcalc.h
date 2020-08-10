#ifndef GCALC_GCALC_H
#define GCALC_GCALC_H


#include "graph.h"
#include <exception>
#include <fstream>
#include <map>
#include <vector>



class GCalc {
    std::map<std::string, Graph> variables;
    std::ifstream* const in;
    std::ofstream* const out;
    const bool ioRedirected;

    typedef Graph (*Operator)(const Graph&, const Graph&);
    static const std::map<char, Operator> operators;

    std::string getCommand() const;
    void parseCommand(const std::string&);
    void runStatement(const std::string&);
    void parseFunctions(const std::string&, unsigned long);
    void assignExpression(const std::string&, unsigned long);
    static Graph parseGraph(std::string);
    static std::pair<Node, Node> parseEdge(std::string);
    std::vector<Graph> handleBrackets(std::string&, unsigned startIndex = 0) const;
    Graph parseVariable(std::string&, std::vector<Graph>&) const;
    Graph parseExpression(std::string&) const;

public:
    class InvalidExpression : public Graph::GraphException {
    public:
        explicit InvalidExpression(const std::string&);
        using GraphException::what;
    };

    GCalc(std::ifstream*, std::ofstream*, bool io = true);
    GCalc();
    GCalc(GCalc&&) noexcept;
    GCalc(const GCalc&) = delete;
    GCalc& operator=(const GCalc&) = delete;
    ~GCalc();

    void saveGraph(const std::string& params) const;
    static Graph loadGraph(const std::string& params);
    void printVariables() const;
    void deleteGraph(std::string& params);
    void run();

};

#endif //GCALC_GCALC_H
