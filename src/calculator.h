#ifndef _CALCULATOR_H
#define _CALCULATOR_H

#include "includes.h"

// functions and structs to calculate user input and display on the calculator

// A very simple calculator trust me.
struct Calculator {
  struct Calculator** buttons; // array of 9 little calculators.
  char res[255];              // result of whatever expressions you been inserting.
  Vector2 corner;             // top left corner
  float len;                  // width/height
  float unit;                 // width/height unit for each small button
};

// recursively clean all unused buttons in the calculator.
// returns true when we are now an empty button ready to be deleted.
bool clearRecursive(struct Calculator* calc) {
  if(!calc) return true;
  // do you like my if-for loops? :3
  if (calc->buttons) for(int i = 0; i < 9; i++) {
    if(!clearRecursive(calc->buttons[i])) return false;
  }
  // Ok so all children were empty I guess.
  return strncmp(calc->res, "0", 255) == 0;
}

// make a new calculator at a given corner with specific length.
struct Calculator* initCalculator(float xPos, float yPos, float len) {
  struct Calculator* calc = (struct Calculator*)calloc(1, sizeof(struct Calculator));
  
  // no value at startup
  calc->buttons = NULL;
  calc->res[0] = '0';
  calc->res[1] = 0;
  
  // main dimensions
  calc->corner = (Vector2){xPos, yPos};
  calc->len = len;
  calc->unit = len/5;
  return calc;
}

// calculate where "clear" button falls under a calculator
struct Rectangle calcClear(struct Calculator* calc) {
  Vector2 dim = calc->corner;
  float unit = calc->unit;
  Rectangle clearButton = {
    .x = dim.x + 4*unit,
    .y = dim.y,
    .width = unit,
    .height = unit
  };
  return clearButton;
}

// calculate where "X" button falls under a calculator
struct Rectangle calcXButton(struct Calculator* calc) {
  Vector2 dim = calc->corner;
  float unit = calc->unit;
  Rectangle xButton = {
    .x = dim.x, 
    .y = dim.y,
    .width = unit,
    .height = unit
  };
  return xButton;
}

// calculate where "display" screen falls under a calculator
struct Rectangle calcDisplay(struct Calculator* calc) {
  Vector2 dim = calc->corner;
  float unit = calc->unit;
  Rectangle xButton = {
    .x = dim.x+unit,
    .y = dim.y+unit,
    .width = calc->len - unit*2,
    .height = calc->len - unit*2
  };
  return xButton;
}

// A modified version of the GuiButton from raylib/raygui.h so it respects camera zoom now.
// it took less than a line.
int cameraGuiButton(Rectangle bounds, const char *text, Camera2D* camera) {
    int result = 0;
    GuiState state = guiState;

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiControlExclusiveMode) {
        // this was literally the only line I had to modify
        //Vector2 mousePoint = GetMousePosition(); 
        Vector2 mousePoint = GetScreenToWorld2D(GetMousePosition(), *camera);     
        if (CheckCollisionPointRec(mousePoint, bounds)) {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else state = STATE_FOCUSED;
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) result = 1;
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(bounds, GuiGetStyle(BUTTON, BORDER_WIDTH), GetColor(GuiGetStyle(BUTTON, BORDER + (state*3))), GetColor(GuiGetStyle(BUTTON, BASE + (state*3))));
    GuiDrawText(text, GetTextBounds(BUTTON, bounds), GuiGetStyle(BUTTON, TEXT_ALIGNMENT), GetColor(GuiGetStyle(BUTTON, TEXT + (state*3))));
    if (state == STATE_FOCUSED) GuiTooltip(bounds);
    //------------------------------------------------------------------

    return result;      // Button pressed: result = 1
}

// Draw a calculator and all its children.
void drawCalculator(struct Calculator* calc, Camera2D* camera) {
  if(!calc) return;
  // dimensions
  Vector2 dim = calc->corner;
  GuiSetStyle(DEFAULT, TEXT_SIZE, calc->unit);

  // first the background colors
  DrawRectangle(dim.x, dim.y, calc->len, calc->len, LIGHTGRAY);

  // Calculate dimensions for the buttons.
  Rectangle xButton = calcXButton(calc);
  Rectangle clearButton = calcClear(calc);
  Rectangle displayScreen = calcDisplay(calc);
  
  // Raygui does not account for zooming by default.
  if(cameraGuiButton(xButton, "X", camera)) {
    fprintf(stderr, "X\n");
    strncat(calc->res, "X", 255);
  }

  // clear the screen.
  if(cameraGuiButton(clearButton, "clear", camera)) {
    fprintf(stderr, "clear\n");
    calc->res[0] = '0';
    calc->res[1] = '\0';
  }

  // clear the screen.
  if(cameraGuiButton(displayScreen, calc->res, camera)) {
    fprintf(stderr, "display\n");
  }

  // draw other ones
  if(!calc->buttons) return;
  for(int i = 0; i < 9; i++) {
    drawCalculator(calc->buttons[i], camera);
  }
}

// add bunch of child nodes to make calculator more COMPLETE after initializing
void giveBirth(struct Calculator* dad) {
  // I structured Calculator in such a way all the necessary variables are already there. 
  dad->buttons = (struct Calculator**)calloc(1, sizeof(struct Calculator));
  Vector2 dim = dad->corner;
  float unit = dad->unit;

  // just hardcoded ratios
  for(int i = 0; i < 3; i++) {
    dad->buttons[i] = initCalculator(dim.x, dim.y + (3-i)*unit, unit);
  }
  for(int i = 1; i < 4; i++) {
    dad->buttons[2+i] = initCalculator(dim.x + i*unit, dim.y, unit);
  }
  for(int i = 1; i < 4; i++) {
    dad->buttons[5+i] = initCalculator(dim.x + 4*unit, dim.y + i*unit, unit);
  }
}


#endif
