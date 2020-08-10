CXX = g++
CPPFLAGS = -std=c++11 -Wall -Werror --pedantic-errors -DNDEBUG
OUT_FLAG = -o
OBJ_FLAG = -c
PROG = gcalc

$(PROG): main.cpp graph/gcalc.h graph/gcalc.cpp stringUtils.o graph.o
	$(CXX) $(CPPFLAGS) $^ $(OUT_FLAG) $@

graph.o: graph/graph.h graph/graph.cpp
	$(CXX) $(CPPFLAGS) $^ $(OBJ_FLAG)

stringUtils.o: stringUtils.h stringUtils.cpp
	$(CXX) $(CPPFLAGS) $^ $(OBJ_FLAG)

libgraph.a: wrappers.o
	ar -rs $@ $^

wrappers.o: graph/graph.h graph/graph.cpp swig/wrappers.h swig/wrappers.cpp
	$(CXX) $(CPPFLAGS) -fPIC $^ $(OBJ_FLAG)

tar:
	zip gcalc graph/* swig/* graph.i main.cpp Makefile stringUtils.cpp stringUtils.h test_in.txt test_out.txt

clean:
	rm -rf $(PROG) *.o *.a *.h.gch graph/*.h.gch swig/*.h.gch
