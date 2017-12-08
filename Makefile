tesh : tmp out/verbose.o out/lecture.o out/execution.o out/main.o out/parser.o out/lexer.o out/commande.o bison flex
	gcc out/verbose.o out/lecture.o out/execution.o out/main.o out/parser.o out/lexer.o out/commande.o -o tesh -Wall -g -DDEBUG

out/main.o : out src/main.c
	gcc -c src/main.c -o out/main.o -Wall

out/execution.o : out src/execution.c
	gcc -c src/execution.c -o out/execution.o -Wall

out/parser.o : out bison src/parser.c
	gcc -c src/parser.c -o out/parser.o -Wall

out/lexer.o : out flex src/lexer.c
	gcc -c src/lexer.c -o out/lexer.o -Wall

out/lecture.o : out src/lecture.c
	gcc -c src/lecture.c -o out/lecture.o -Wall

out/commande.o : out src/commande.c
	gcc -c src/commande.c -o out/commande.o -Wall

out/verbose.o : out src/verbose.c
	gcc -c src/verbose.c -o out/verbose.o -Wall

bison : src/parser.y
	bison -d --verbose --debug --no-lines -o src/parser.c src/parser.y -Wall

flex : bison src/lexer.l
	flex --verbose --debug --header-file=src/lexer.h --noline -o src/lexer.c src/lexer.l

out :
	mkdir out

tmp :
	mkdir tmp

clean :
	rm out/* -f

add :
	git add src/*
