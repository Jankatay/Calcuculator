Calculate your own calculator.

# Usage.
Raise 0^0 to get started. Then zoom out and use the result as button.  

Buttons with "X" in them will be applied to what's on the display. So if screen has "+" and you press on a button with "2x3" on it you will get "5."
They can be applied to numbers or a mix of numbers and operators as well.

Use "C" to clear screen.

# Todo
Currently I would much rather start a new project than improve this one but in the future when I write again from scratch I will be looking out for these.  
- Zooming in too much causes freezing.
- Memory management is not as efficient as it can/should be.
- Run through valgrind.
- Add operator precedence.
- Use a smaller graphics library. Current executable is 1000kb on linux and 1004 kb on windows. Did manage getting down to 286kb on linux using packers.

# Credits
## Graphics libraries.
- raylib - https://github.com/raysan5/raylib
- raygui - https://github.com/raysan5/raygui

## Cross-compiling tools.
- mingw-w64 - https://www.mingw-w64.org/

## Helpful links used
- Overall C - https://linux.die.net/man/3/
- camera_mouse_zoom - https://github.com/raysan5/raylib/blob/master/examples/core/core_2d_camera_mouse_zoom.c
- cross-compiling - https://stackoverflow.com/questions/69863663/how-do-i-link-and-compile-raylib-on-windows#71320067
