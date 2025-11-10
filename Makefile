CC = gcc -Wall -Wextra -Werror 

run: main
	./main

main: main.c
	$(CC) -o $@ $^ -lraylib
