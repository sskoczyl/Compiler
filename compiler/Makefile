stages: bison_compile lex_compile cpp_compile clean

cpp_compile:
	g++ -o kompilator parser.tab.cpp lexer.l.c src/memory.cpp src/operations.cpp

lex_compile: lexer.l
	lex -o lexer.l.c lexer.l

bison_compile: parser.ypp
	bison -d parser.ypp

clean:
	rm parser.tab.* lexer.l.*