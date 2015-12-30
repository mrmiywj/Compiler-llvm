prog: lex.yy.o y.tab.o main.o
	clang++ -std=c++11  main.o lex.yy.o y.tab.o `llvm-config --cxxflags --ldflags --system-libs --libs core mcjit native` -o prog

main.o: main.cpp
	clang++  -c -std=c++11 main.cpp `llvm-config --cxxflags --ldflags --system-libs --libs core mcjit native`  -o main.o 

lex.yy.o: lex.yy.c y.tab.h
	gcc -c -std=c99 lex.yy.c

y.tab.o: y.tab.c
	gcc -c -std=c99 y.tab.c

y.tab.c y.tab.h: smallc.y
	yacc -d smallc.y

lex.yy.c: smallc.l
	lex smallc.l

clean:
	rm  lex.* y.tab.*
