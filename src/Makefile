# raygui complains about unused parameters/functions
CFLAGS = -Os -flto -ffunction-sections -Wl,--gc-sections -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-function -D__STDC_WANT_IEC_60559_BFP_EXT__


run: main
	./main

main: main.c base.h calculator.h draw.h includes.h operations.h
	gcc $(CFLAGS) -Lraylib -o $@ $^ -lraylib -lm 
	strip $@

windows/main.exe: main.c base.h calculator.h draw.h includes.h operations.h
	x86_64-w64-mingw32-gcc $(CFLAGS) -Lwindows -o $@ main.c -lraylib -lopengl32 -lgdi32 -lwinmm -lm
	strip $@

.PHONY: clean
clean: 
	rm -f main
	rm -f windows/main.exe
