# raygui complains about unused parameters/functions
CFLAGS = -Os -flto -ffunction-sections -Wl,--gc-sections -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-function -D__STDC_WANT_IEC_60559_BFP_EXT__

run: main
	./main

main: main.c base.h calculator.h draw.h
	gcc $(CFLAGS) -Lraylinux -o $@ $^ -lraylib -lm 
	strip $@

windows: main.c base.h calculator.h draw.h
	 zig cc -o test.exe -target x86_64-windows-gnu  -D_GNU_SOURCE -I/usr/include $(CFLAGS) -w -Lraylib -o $@ $^ -lraywindows -lm 


.PHONY: clean
clean: 
	rm -f main
