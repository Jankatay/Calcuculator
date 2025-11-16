# raygui complains about unused parameters/functions
CFLAGS = -Os -flto -ffunction-sections -Wl,--gc-sections -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-function -D__STDC_WANT_IEC_60559_BFP_EXT__

run: calcuculator
	./$<

windows: calcuculator.exe
	wine ./$<

calcuculator: main.c base.h calculator.h draw.h includes.h operations.h
	gcc $(CFLAGS) -g -Lraylib -o $@ $^ -lraylib -lm 
	cp $@ release
	strip release/$@

calcuculator.exe: main.c base.h calculator.h draw.h includes.h operations.h
	x86_64-w64-mingw32-gcc -g $(CFLAGS) -Lraylib -o $@ main.c -lraywindows -lopengl32 -lgdi32 -lwinmm -lm
	cp $@ release
	strip release/$@

.PHONY: clean
clean: 
	rm -f calcuculator calcuculator.exe
	rm -f release/*
