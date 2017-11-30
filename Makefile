tesh : out/lecture.o out/analyse.o out/execution.o out/main.o
	gcc out/lecture.o out/analyse.o out/execution.o out/main.o -o tesh

out/main.o : out src/main.c
	gcc -c src/main.c -o out/main.o

out/lecture.o : out src/lecture.c
	gcc -c src/lecture.c -o out/lecture.o -Wall

out/analyse.o : out src/analyse.c
	gcc -c src/analyse.c -o out/analyse.o -Wall

out/execution.o : out src/execution.c
	gcc -c src/execution.c -o out/execution.o -Wall

out :
	mkdir out

clean :
	rm out/* -f

add :
	git add src/*
