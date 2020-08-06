CXX = g++
CPPFLAGS = -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG
EXEC = gcalc
OUT_FLAG = -o
OBJ_FLAG = -c
LIB_FLAGS = -L. -lgraph

${EXEC}: main.cpp graph.a
	${CXX} ${CPPFLAGS} $^ ${OUT_FLAG} $@

graph.o: graph.h graph.cpp
	${CXX} ${CPPFLAGS} $^ ${OBJ_FLAG}

# libgraph.a:

# tar:

clean:
	rm -rf ${EXEC} *.o
