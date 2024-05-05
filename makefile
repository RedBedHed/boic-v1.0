
comp: analyzer.o parse_expression.o compiler.o declare.o control_flow.o expression.o function.o library.o
	g++ -ggdb3 -Wall -o $@ $^

parse_expression.o: parse_expression.cpp compiler.h analyzer.h
	g++ -ggdb3 -Wall -c parse_expression.cpp

analyzer.o: analyzer.cpp analyzer.h
	g++ -ggdb3 -Wall -c analyzer.cpp

declare.o: declare.cpp compiler.h analyzer.h
	g++ -ggdb3 -Wall -c declare.cpp

control_flow.o: control_flow.cpp compiler.h analyzer.h
	g++ -ggdb3 -Wall -c control_flow.cpp

expression.o: expression.cpp compiler.h analyzer.h
	g++ -ggdb3 -Wall -c expression.cpp

function.o: function.cpp compiler.h analyzer.h
	g++ -ggdb3 -Wall -c function.cpp

library.o: library.cpp compiler.h analyzer.h
	g++ -ggdb3 -Wall -c library.cpp

compiler.o: compiler.cpp compiler.h analyzer.h
	g++ -ggdb3 -Wall -c compiler.cpp

clean:
	rm comp *.o
