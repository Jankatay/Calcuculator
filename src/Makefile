# raygui complains about unused parameters/functions
CFLAGS = -Os -flto -ffunction-sections -Wl,--gc-sections -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-function -D__STDC_WANT_IEC_60559_BFP_EXT__


run: main
	./release/main

windows: main.exe
	wine ./release/main.exe

main: main.c base.h calculator.h draw.h includes.h operations.h
	gcc $(CFLAGS) -Lraylib -o $@ $^ -lraylib -lm 
	cp $@ release
	strip release/$@

main.exe: main.c base.h calculator.h draw.h includes.h operations.h
	x86_64-w64-mingw32-gcc $(CFLAGS) -Lraylib -o $@ main.c -lraywindows -lopengl32 -lgdi32 -lwinmm -lm
	cp $@ release
	strip release/$@

.PHONY: clean
clean: 
	rm -f main main.exe
	rm -f release/* 
