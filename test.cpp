#include "graph.h"
#include <iostream>
#include <map>

#define ASSERT_TEST(b) do { \
        if (!(b)) { \
                printf("\nAssertion failed at %s:%d %s ",__FILE__,__LINE__,#b); \
                return false; \
        } \
} while (0)

#define RUN_TEST(test) do { \
        printf("Running "#test"... "); \
        std::cout << std::endl; \
        if (test()) { \
            printf("[OK]\n");\
        } else { \
                printf("[Failed]\n"); \
        } \
        std::cout << std::endl;\
} while(0)


using Edge = Graph::Edge;
const int SIZE = 6;
const Node nodes[SIZE]{"a", "B", "c23", "d10", "e9", "f10G11h12I13"};


bool testEdge() {
    Edge e("Hello", "World"), copy(e);
    ASSERT_TEST(e.src == "Hello");
    ASSERT_TEST(e.dest == "World");
    ASSERT_TEST(&e != &copy);
    ASSERT_TEST(copy.src == e.src);
    ASSERT_TEST(copy.dest == e.dest);
    ASSERT_TEST(copy == e);
    ASSERT_TEST(e == copy);
    ASSERT_TEST(e.toString() == "Hello World");
    ASSERT_TEST(!(e < Edge("AAAA", "BBBB")));
    ASSERT_TEST(!(e < Edge("Hello", "Mom")));
    ASSERT_TEST(e < Edge("Wonderful", "World"));
    Edge assign = e;
    ASSERT_TEST(&e != &assign);
    ASSERT_TEST(&e != &copy);
    ASSERT_TEST(assign.src == e.src);
    ASSERT_TEST(assign.dest == e.dest);
    return true;
}

bool testCtorAssignment() {
    Graph g1;
    ASSERT_TEST(g1.getNodes().empty());
    Graph g2(g1);
    ASSERT_TEST(g2.getNodes().empty());
    Graph g3 = g1;
    ASSERT_TEST(g3.getNodes().empty());
    return true;
}

bool testAddRemoveNode() {
    const Node bracketNodes[SIZE]{"[]", "[a[b[c]d]e]", "[;;;[;;];]", "[v1;w1]", "[[[[[]]]]]", "[[[a1;a2];a3];a4]"};
    const Node invalidNames[]{"p+us", "m-nus", "$ollar", "excla!m", "spa ce", "t\tab", "new\nline",
                              "@t", "#tag", "perc%nt", "c^rr^t", "sl/ash", "back\\slash", "[", "[]]", "]",
                              "a;", "a[b;[c;]d;;;]e;f"};
    Graph g1;
    for(const Node& n : nodes) {
        g1.addNode(n);
    }
    ASSERT_TEST(g1.getNodes().size() == SIZE);
    for(int i = 0; i < SIZE; i++) {
        ASSERT_TEST(g1.getNodes().find(nodes[i]) != g1.getNodes().end());
    }
    g1.removeNode("randomnode");
    ASSERT_TEST(g1.getNodes().size() == SIZE);
    for(int i = 0; i < SIZE; i++) {
        g1.removeNode(nodes[i]);
        ASSERT_TEST((int) g1.getNodes().size() == SIZE - i - 1);
        ASSERT_TEST(g1.getNodes().find(nodes[i]) == g1.getNodes().end());
    }
    for(const Node& n : bracketNodes) {
        g1.addNode(n);
        ASSERT_TEST(g1.getNodes().find(n) != g1.getNodes().end());
    }
    for(const Node& n : invalidNames) {
        try {
            g1.addNode(n);
        } catch(const Graph::InvalidName& in) {
            std::cout << in.what() << std::endl;
            ASSERT_TEST(g1.getNodes().size() == SIZE);
            ASSERT_TEST(g1.getNodes().find(n) == g1.getNodes().end());
            continue;
        }
        ASSERT_TEST(false);
    }
    for(const Node& n : invalidNames) {
        try {
            g1.removeNode(n);
        } catch(const std::exception& e) {
            ASSERT_TEST(false);
        }
    }
    return true;
}

bool testAddRemoveEdge() {
    Graph g1;
    try {
        ASSERT_TEST(g1.neighbours("Hello").empty());
    } catch(const std::exception& nodeNotFound) {
        std::cout << nodeNotFound.what() << std::endl;
    }
    try {
        ASSERT_TEST(!g1.adjacent("Test", "World"));
    } catch(const std::exception& nodeNotFound) {
        std::cout << nodeNotFound.what() << std::endl;
    }
    for(const Node& n : nodes) {
        g1.addNode(n);
    }
    for(int i = 1; i < SIZE; i++) {
        ASSERT_TEST(!g1.adjacent(nodes[i- 1], nodes[i]));
        ASSERT_TEST(g1.neighbours(nodes[i]).empty());
    }
    for(int i = 1; i < SIZE; i++) {
        g1.addEdge(nodes[i - 1], nodes[i]);
        ASSERT_TEST(g1.adjacent(nodes[i - 1], nodes[i]));
        ASSERT_TEST(!g1.adjacent(nodes[i], nodes[i - 1]));
        ASSERT_TEST(g1.neighbours(nodes[i - 1]).size() == 1);
        ASSERT_TEST(g1.neighbours(nodes[i]).empty());
    }
    for(int i = 0; i < SIZE - 1; i++) {
        std::set<Node> neighbours = g1.neighbours(nodes[i]);
        ASSERT_TEST( neighbours.size() == 1);
        ASSERT_TEST(neighbours.find(nodes[i + 1]) != neighbours.end());
    }
    for(int i = 1; i < SIZE - 1; i++) {
        g1.removeEdge(nodes[i - 1], nodes[i]);
        ASSERT_TEST(!g1.adjacent(nodes[i - 1], nodes[i]));
        ASSERT_TEST(!g1.adjacent(nodes[i], nodes[i - 1]));
        ASSERT_TEST(g1.neighbours(nodes[i - 1]).empty());
        std::cout << g1.neighbours(nodes[i]).size() << ", " << i << std::endl;
        ASSERT_TEST(g1.neighbours(nodes[i]).size() == 1);
    }
    return true;
}

int main() {
    RUN_TEST(testEdge);
    RUN_TEST(testCtorAssignment);
    RUN_TEST(testAddRemoveNode);
    RUN_TEST(testAddRemoveEdge);
    return 0;
}
