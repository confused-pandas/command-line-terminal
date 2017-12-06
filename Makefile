tesh : out/execution.o out/main.o out/parser.o out/lexer.o
	gcc out/execution.o out/main.o out/parser.o out/lexer.o -o tesh

out/main.o : out src/main.c
	gcc -c src/main.c -o out/main.o

out/execution.o : out src/execution.c
	gcc -c src/execution.c -o out/execution.o -Wall

out/parser.o : out bison
	gcc -c src/parser.c -o out/parser.o -Wall

out/lexer.o : out flex
	gcc -c src/lexer.c -o out/lexer.o -Wall

bison :
	bison -d -o src/parser.c src/parser.y

flex : bison
	flex -o src/lexer.c src/lexer.l

out :
	mkdir out

clean :
	rm out/* -f

add :
	git add src/*
