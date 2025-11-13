# raygui complains about unused parameters/functions
CC = gcc -Oz -flto -ffunction-sections -Wl,--gc-sections -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-function

run: main
	./main

main: main.c base.h calculator.h draw.h
	$(CC) -Lraylib -o $@ $^ -lraylib -lm 
	strip $@

.PHONY: clean
clean: 
	rm -f main
