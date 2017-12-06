tesh : out/execution.o out/main.o out/parser.o out/lexer.o out/commande.o bison flex
	gcc out/execution.o out/main.o out/parser.o out/lexer.o out/commande.o -o tesh -Wall -g -DDEBUG

out/main.o : out src/main.c
	gcc -c src/main.c -o out/main.o -Wall

out/execution.o : out src/execution.c
	gcc -c src/execution.c -o out/execution.o -Wall

out/parser.o : out bison
	gcc -c src/parser.c -o out/parser.o -Wall

out/lexer.o : out flex
	gcc -c src/lexer.c -o out/lexer.o -Wall

out/commande.o : out
	gcc -c src/commande.c -o out/commande.o -Wall

bison : src/parser.y
	bison -d --no-lines --verbose -o src/parser.c src/parser.y -Wall

flex : bison src/lexer.l
	flex --debug --header-file=src/lexer.h --noline -o src/lexer.c src/lexer.l

out :
	mkdir out

clean :
	rm out/* -f

add :
	git add src/*
