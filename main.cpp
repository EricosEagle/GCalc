#include "graph.h"
#include "stringutils.h"
#include <algorithm>
#include <cassert>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#define MESSAGE "Gcalc> "

typedef std::map<std::string, Graph> GraphMap;
std::vector<Graph> handleBrackets(std::string&, GraphMap&);

Graph parseExpression(std::string, GraphMap&);

class InvalidExpression : public std::invalid_argument {
public:
    using std::invalid_argument::invalid_argument;
    using std::invalid_argument::what;
};

void redirectIO(const char* inPath, const char* outPath, std::fstream& in, std::fstream& out) {
    in.open(inPath, std::ios::in);
    out.open(outPath, std::ios::out);
    std::cin.rdbuf(in.rdbuf());
    std::cout.rdbuf(out.rdbuf());
}

std::string getCommand(bool ioRedirect) {
    std::string command;
    if(!ioRedirect) {
        std::cout << MESSAGE;
    }
    std::getline(std::cin, command);
    return stringUtils::removeWhitespace(command);
}

static void removeFirstLast(std::string& s1, std::string& s2) {
    s1.erase(0, 1);
    s2.pop_back();
}

std::pair<Node, Node> parseEdge(const std::string& edge) {
    assert(edge.front() == '<' && edge.back() == '>');
    std::pair<Node, Node> endpoints;
    try {
        endpoints = stringUtils::seperate(edge, ',');
    } catch (const std::invalid_argument&) {
        throw InvalidExpression("'" + edge + "' is not a valid edge.");
    }
    removeFirstLast(endpoints.first, endpoints.second);
    return endpoints;
}

Graph parseGraph(const std::string& graph) {
    assert(graph.front() == '{' && graph.back() == '}');
    std::pair<std::string, std::string> seperated;
    try {
        seperated = stringUtils::seperate(graph, '|');
    } catch (const std::invalid_argument&) {
        throw InvalidExpression("'" + graph + "' is not a valid graph.");
    }
    removeFirstLast(seperated.first, seperated.second);
    std::vector<std::string> nodes = stringUtils::split(seperated.first, ","),
        edges = stringUtils::split(seperated.second, ">,"); // Only seperates edges, not the nodes in them
    Graph out;
    for(const Node& node : nodes) {
        out.addNode(node);
    }
    for(const std::string& edge : edges) {
        std::pair<Node, Node> endPoints = parseEdge(edge + ">"); // Add the closing arrow bracket back
        out.addEdge(endPoints.first, endPoints.second);
    }
    return out;
}

std::vector<Graph> handleBrackets(std::string& e, GraphMap& literals) {
    bool invalidExpression = false;
    int bracketCounter = 0;
    std::vector<Graph> temps;
    std::string current, newExpression;
    for(unsigned i = 0; !invalidExpression && i < e.size(); i++) {
        switch(e[i]) {
            case '(':
                bracketCounter++;
                break;
            case ')':
                bracketCounter--;
                if(bracketCounter == 0) {
                    newExpression += ("$" + std::to_string(temps.size()) + "$");
                    temps.push_back(parseExpression(current, literals));
                    current.clear();
                }
            default:
                std::string& toUpdate = (bracketCounter > 0) ? current:newExpression;
                toUpdate.push_back(e[i]);
                invalidExpression = !isalnum(e[i]);
        }
        invalidExpression = invalidExpression || bracketCounter < 0;
    }
    e = newExpression;
    return temps;
}

Graph parseExpression(std::string e, GraphMap& literals) {
    Graph out;
    std::vector<Graph> temps = handleBrackets(e, literals);

    // TODO: Implement function
    return out;
}

void assignLiteral(GraphMap& literals, const std::string& command, unsigned long equals_index) {
    Node node = command.substr(0, equals_index);
    if(!isalpha(node[0]) || !std::all_of(node.begin() + 1, node.end(), isalnum)) {
        throw Graph::InvalidName("Invalid graph name: \"" + node + "\".");
    }
    std::string expression = command.substr(equals_index + 1);
    literals[node] = parseExpression(expression, literals);
}

void printLiterals(const GraphMap& literals) {
    for(const auto& literal : literals) {
        std::cout << literal.first << std::endl;
    }
}

void parseFunctions(const std::string& command, GraphMap& literals, unsigned long bracket_index) {
    std::string function = command.substr(0, bracket_index), param = command.substr(bracket_index + 1);
    param.pop_back(); // Remove end bracket ')'
    if(function == "delete") {
        auto iter = literals.find(param);
        if(iter == literals.end()) {
            throw std::invalid_argument("Undefined variable '" + param + "'.");
        }
        literals.erase(iter);
        return;
    }
    Graph g = parseExpression(param, literals);
    if(function == "print") {
        std::cout << g << std::endl;
    } /*else if(function == "save") {

    } else if(function == "load") {

    } */
    else {
        throw std::invalid_argument("Unrecognised command '" + command + "'.");
    }
}

void parseCommand(const std::string& command, GraphMap& literals) {
    unsigned long index; // string.find() returns unsigned long instead on int
    if(command == "who") {
        printLiterals(literals);
    } else if(command == "reset") {
        literals.clear();
    } else if((index = command.find('(')) !=  std::string::npos && command.find(')') == command.length() - 1) {
        parseFunctions(command, literals, index);
    } else if((index = command.find('=')) != std::string::npos) {
        assignLiteral(literals, command, index);
    } else if(command != "quit") {
        throw std::invalid_argument("Unrecognised command '" + command + "'");
    }
}

void gcalc(bool ioRedirect = false) {
    GraphMap literals;
    std::string command;
    while(std::cin.good() && command != "quit") {
        command = getCommand(ioRedirect);
        try {
            parseCommand(command, literals);
        } catch(const std::invalid_argument& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
}

int main(int argc, char** argv) {
    std::fstream in, out;
    bool ioRedirect = argc == 3;
    if(ioRedirect) {
        redirectIO(argv[1], argv[2], in, out);
    }
    gcalc(ioRedirect);
    in.close();
    out.close();
    return 0;
}
