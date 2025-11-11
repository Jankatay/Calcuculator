# raygui complains about unused parameters/functions
CC = gcc -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-function

run: main
	./main

main: base.h calculator.h main.c
	$(CC) -o $@ $^ -lraylib -lm

.PHONY: clean
clean: 
	rm -f main
