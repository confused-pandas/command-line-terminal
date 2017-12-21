tesh : tmp out/liberer.o out/is_file.o out/verbose.o out/lecture.o out/execution.o out/main.o out/parser.o out/lexer.o out/commande.o bison flex
	gcc out/liberer.o out/is_file.o out/verbose.o out/lecture.o out/execution.o out/main.o out/parser.o out/lexer.o out/commande.o -o tesh -Wall -g -DDEBUG -ldl

out/main.o : out src/main.c
	gcc -c src/main.c -o out/main.o -Wall -g -DDEBUG -ldl

out/execution.o : out src/execution.c
	gcc -c src/execution.c -o out/execution.o -Wall -g

out/parser.o : out bison src/parser.c
	gcc -c src/parser.c -o out/parser.o -Wall -g

out/lexer.o : out flex src/lexer.c
	gcc -c src/lexer.c -o out/lexer.o -Wall -g

out/lecture.o : out src/lecture.c
	gcc -c src/lecture.c -o out/lecture.o -Wall -g

out/commande.o : out src/commande.c
	gcc -c src/commande.c -o out/commande.o -Wall -g

out/verbose.o : out src/verbose.c
	gcc -c src/verbose.c -o out/verbose.o -Wall -g

out/is_file.o : out src/is_file.c
	gcc -c src/is_file.c -o out/is_file.o -Wall -g

out/liberer.o : out src/liberer.c
	gcc -c src/liberer.c -o out/liberer.o -Wall -g

bison : src/parser.y
	bison -d -o src/parser.c src/parser.y -Wall

flex : bison src/lexer.l
	flex --header-file=src/lexer.h -o src/lexer.c src/lexer.l

out :
	mkdir out

tmp :
	mkdir tmp

clean :
	rm out/* -f

add :
	git add src/*
