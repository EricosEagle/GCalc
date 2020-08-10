#include "gcalc.h"
#include "../stringUtils.h"
#include <algorithm>
#include <cassert>
#include <string>
#include <fstream>
#include <regex>

#define MESSAGE "Gcalc> "
#define COMPLEMENT_OPERATOR '!'

#define GET_VARIABLE(out, name) auto (out) = variables.find(name); \
if((out) == variables.end()) { \
throw Graph::GraphException(name, "is undefined."); \
}

const std::set<std::string> statements {"who", "reset", "quit"};

const std::map<char, GCalc::Operator> GCalc::operators {
        {'+', Graph::unite},
        {'^', Graph::intersection},
        {'-', Graph::difference},
        {'*', Graph::product}
};

static bool isStatement(const std::string& str) {
    return statements.find(str) != statements.end();
}

static bool isFunction(const std::string& str) {
    return str == "print" || str == "delete" || str == "save" || str == "load";
}

GCalc::GCalc(std::ifstream* is, std::ofstream* os, bool io) : variables(), in(is), out(os), ioRedirected(io) {
    if(ioRedirected) {
        std::cin.rdbuf(in->rdbuf());
        std::cout.rdbuf(out->rdbuf());
    }
}

GCalc::GCalc(GCalc&& g) noexcept : variables(std::move(g.variables)), in(g.in), out(g.out), ioRedirected(g.ioRedirected) {}

GCalc::GCalc() : GCalc(nullptr, nullptr, false) {}
GCalc::~GCalc() {
    if(ioRedirected) {
        in->close();
        out->close();
        delete in;
        delete out;
    }
}

GCalc::InvalidExpression::InvalidExpression(const std::string& e): Graph::GraphException(e, "is not a valid expression.") {}

void GCalc::printVariables() const {
    for(const auto& literal : variables) {
        std::cout << literal.first << std::endl;
    }
}

void GCalc::deleteGraph(std::string& params) {
    GET_VARIABLE(iter, params);
    variables.erase(iter);
}

void GCalc::saveGraph(const std::string& params) const {
    unsigned long index = params.rfind(',');
    if(index == std::string::npos) {
        throw std::invalid_argument("No file specified!");
    }
    std::string expression(params.begin(), params.begin() + index), fName(params.begin() + index + 1, params.end());
    try {
        parseExpression(expression).save(fName);
    } catch(const std::ifstream::failure&) {
        throw std::invalid_argument("Could not open the file.");
    }
}

Graph GCalc::loadGraph(const std::string& params) {
    std::ifstream graphFile;
    try {
        return Graph::load(params);
    } catch(const std::ifstream::failure&) {
        throw std::invalid_argument("Could not open '" + params + "'.");
    }

}

void GCalc::runStatement(const std::string& statement) {
    if(statement == "who") {
        printVariables();
    } else if(statement == "reset") {
        variables.clear();
    }
}

std::pair<Node, Node> GCalc::parseEdge(std::string edge) {
    assert(edge.front() == '<' && edge.back() == '>');
    std::string originalEdge(edge);
    edge.erase(0, 1);
    edge.pop_back();
    auto endpoints = stringUtils::partition(edge, ",");
    if(std::any_of(endpoints.begin(), endpoints.end(), [](const std::string& s){return s.empty();})) {
        throw Graph::Edge::EdgeError("'" + originalEdge + "' is not a valid edge!");
    }
    return {endpoints[0], endpoints[2]};
}

Graph GCalc::parseGraph(std::string graph) {
    assert(graph.front() == '{');
    if(graph.back() != '}') {
        throw InvalidExpression(graph);
    }
    graph.erase(0, 1);
    graph.pop_back();
    auto partitioned = stringUtils::partition(graph, "|");
    std::string nodes = partitioned[0], edges = partitioned[2];
    Graph result;
    if(!nodes.empty()) {
        for(const Node& node : stringUtils::split(nodes, ",")) {
            if(result.containsNode(node)) {
                throw Graph::GraphException(node, "cannot be in the graph more than one time!");
            }
            result.addNode(node);
        }
    }
    if(!partitioned.back().empty()) {
        std::vector<std::string> e = stringUtils::split(partitioned.back(), ">,"); // Only seperates edges, not the nodes in them
        for(unsigned i = 0; i < e.size(); i++) {
            std::string edge = e[i] + (i == e.size() - 1 ? "":">");
            std::pair<Node, Node> endPoints = parseEdge(edge); // Add the closing arrow bracket back
            if(result.adjacent(endPoints.first, endPoints.second)) {
                throw Graph::GraphException(edge, "is already in the graph!");
            }
            result.addEdge(endPoints.first, endPoints.second);
        }
    }
    return result;
}

std::vector<Graph> GCalc::handleBrackets(std::string& e, unsigned startIndex) const {
    bool validExpression = true;
    int bracketCounter = 0;
    std::vector<Graph> temps;
    std::string current, newExpression;
    for(unsigned i = 0; validExpression && i < e.size(); i++) {
        switch(e[i]) {
            case '(':
                if(bracketCounter > 0) {
                    current.push_back(e[i]);
                }
                bracketCounter++;
                break;
            case ')':
                bracketCounter--;
                if(bracketCounter == 0) {
                    newExpression += ("$" + std::to_string(startIndex + temps.size()));
                    temps.push_back(parseExpression(current));
                    current.clear();
                    break;
                }
            default:
                std::string& toUpdate = (bracketCounter > 0) ? current:newExpression;
                toUpdate.push_back(e[i]);
        }
        validExpression = bracketCounter >= 0;
    }
    if(!validExpression) {
        throw InvalidExpression(e);
    }
    e = newExpression;
    return temps;
}

static Graph& getTemp(std::vector<Graph>& temps, std::string& varName) {
    assert(varName.front() == '$');
    varName.erase(0, 1); // Remove $
    assert(isDigit(varName));
    int index = std::stoi(varName);
    assert(index >= 0 && index < temps.size());
    return temps[index];
}

Graph GCalc::parseVariable(std::string& varName, std::vector<Graph>& temps) const {
    Graph result;
    bool complement = false;
    if(varName.front() == COMPLEMENT_OPERATOR)  {
        complement = true;
        varName.erase(0, 1);
    }
    switch(varName.front()) {
        case '{':
            result = parseGraph(varName);
            break;
        case '$':
            result = getTemp(temps, varName);
            break;
        default:
            GET_VARIABLE(iter, varName);
            result = iter->second;
            break;
    }
    return complement ? result.complement():result;
}

static std::vector<Graph> loadFiles(std::string& expression) {
    std::vector<Graph> temps;
    std::regex loadFunction("load\\((.*)\\)");
    auto iter = std::sregex_iterator(expression.begin(), expression.end(), loadFunction);
    auto endIter = std::sregex_iterator();
    while(iter != endIter) {
        expression = expression.substr(0, iter->position(0)) +
                     "$" +
                     std::to_string(temps.size()) +
                     iter->suffix().str();
        temps.push_back(GCalc::loadGraph(iter->str(1)));
        iter++;
    }
    return temps;
}

Graph GCalc::parseExpression(std::string& expression) const {
    bool validExpression = (expression.find('$') == std::string::npos); // Make sure there are no $ signs
    std::string newExpression = expression;
    std::vector<Graph> temps(loadFiles(newExpression)), bracketVars(handleBrackets(newExpression, temps.size()));
    temps.insert(temps.end(), bracketVars.begin(), bracketVars.end());
    auto nextOperation = operators.begin();
    Operator currentOperation = nullptr;
    auto startIter = newExpression.begin();
    auto endIter = std::find_if(startIter, newExpression.end(),
                                 [&](const char& c){return (nextOperation = operators.find(c)) != operators.end();});
    Graph result;
    while(validExpression) {
        std::string variableName(startIter, endIter);
        Graph operand = parseVariable(variableName, temps);
        result = (currentOperation == nullptr) ? operand:currentOperation(result, operand);
        if(nextOperation == operators.end()) {
            break;
        }
        currentOperation = nextOperation->second;
        startIter = endIter + 1;
        endIter = std::find_if(startIter, newExpression.end(),
                               [&](const char& c){return (nextOperation = operators.find(c)) != operators.end();});
    }
    if(!validExpression) {
        throw InvalidExpression(expression);
    }
    return result;
}

void GCalc::parseFunctions(const std::string& command, unsigned long bracket_index) {
    std::string func = command.substr(0, bracket_index), params = command.substr(bracket_index + 1);
    params.pop_back(); // Remove end bracket ')'
    if(func == "print") {
        std::cout << parseExpression(params) << std::endl;
    } else if(func == "delete") {
        deleteGraph(params);
    } else if(func == "save") {
        saveGraph(params);
    } else {
        throw Graph::GraphException(command, "is not a valid command.");
    }
}

void GCalc::assignExpression(const std::string& command, unsigned long equals_index) {
    Node variableName = command.substr(0, equals_index);
    if(isStatement(variableName) ||
        isFunction(variableName) ||
        !isalpha(variableName[0]) ||
        !std::all_of(variableName.begin() + 1, variableName.end(), isalnum)) {
        throw Graph::InvalidName(variableName);
    }
    std::string expression = command.substr(equals_index + 1);
    variables[variableName] = parseExpression(expression);
}

void GCalc::parseCommand(const std::string& command) {
    unsigned long index; // string.find() returns unsigned long instead on int
    auto iter = statements.find(command);
    if(iter != statements.end()) {
        runStatement(command);
    } else if((index = command.find('=')) != std::string::npos) {
        assignExpression(command, index);
    } else if((index = command.find('(')) !=  std::string::npos && command.rfind(')') == command.length() - 1) {
        parseFunctions(command, index);
    } else {
        throw Graph::GraphException(command, "is not a valid command!");
    }
}

std::string GCalc::getCommand() const {
    std::string command;
    if(!ioRedirected) {
        std::cout << MESSAGE;
    }
    std::getline(std::cin, command);
    return stringUtils::removeWhitespace(command);
}

void GCalc::run() {
    std::string command;
    while(std::cin.good() && command != "quit") {
        try {
            command = getCommand();
            if(!command.empty()) {
                parseCommand(command);
            }
        } catch(const std::invalid_argument& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
}
