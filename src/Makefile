# raygui complains about unused parameters/functions
CC = gcc -g -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-function

run: main
	./main

main: main.c base.h calculator.h
	$(CC) -o $@ $^ -lraylib -lm

.PHONY: clean
clean: 
	rm -f main
