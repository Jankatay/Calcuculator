# raygui complains about unused parameters/functions
CFLAGS = -Lraylib -Os -flto -ffunction-sections -Wl,--gc-sections -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-function -D__STDC_WANT_IEC_60559_BFP_EXT__


run: main
	./main

main: main.c base.h calculator.h draw.h includes.h operations.h
	gcc $(CFLAGS) -Lraylinux -o $@ $^ -lraylib -lm 
	strip $@

.PHONY: clean
clean: 
	rm -f main
