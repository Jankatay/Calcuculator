#ifndef _DRAW_H
#define _DRAW_H

#include "includes.h"
#include "base.h"
#include "calculator.h"

// calculate where "clear" button falls under a calculator
bool computeClear(struct Calculator* calc, Camera2D camera) {
  // calculate dimensions
  Vector2 dim = calc->corner;
  float unit = calc->unit;
  Rectangle clearButton = {
    .x = dim.x + 4*unit,
    .y = dim.y,
    .width = unit,
    .height = unit
  };
  // draw the button
  if(cameraGuiButton(clearButton, "C", camera)) {
    calc->res[0] = '0';
    calc->res[1] = '\0';
    calc->noval = true;
    return true;
  }
  return false;
}

// add a string to a display
void addStringToDisplay(const char* str, struct Calculator* calc) {
  if(!calc || !str) return;
  // there was an input and screen was clear
  if(calc->noval) {
    strncpy(calc->res, str, 128);
    calc->noval = false;
    return;
  }
  // there was an input and screen had stuff on it.
  int distance = 128 - strnlen(str, 128);
  strncat(calc->res, str, distance);
}

// calculate where "label" button falls under a calculator
bool fixedButton(struct Calculator* calc, const char* label, float xPos, float yPos, Camera2D camera) {
  // calculate the dimensions
  Vector2 dim = calc->corner;
  float unit = calc->unit;
  Rectangle button = {
    .x = dim.x + xPos*unit, 
    .y = dim.y + yPos*unit,
    .width = unit,
    .height = unit
  };
  // draw the button
  if(cameraGuiButton(button, label, camera)) {
    addStringToDisplay(label, calc);
    calc->noval = false;
    return true;
  }
  return false;
}

// calculate where an operator falls under a calculator.
bool fixedOperator(struct Calculator* calc, const char* label, float xPos, float yPos, Camera2D camera) {
  // calculate the dimensions
  Vector2 dim = calc->corner;
  float unit = calc->unit;
  Rectangle button = {
    .x = dim.x + xPos*unit, 
    .y = dim.y + yPos*unit,
    .width = unit,
    .height = unit
  };
  // draw the button
  if(cameraGuiButton(button, label, camera)) {
    float len = strnlen(label, 128);
    if(calc->noval) strncpy(calc->res, label, 128);
    else strncat(calc->res, label, 128-len);
    calc->noval = false;
    return true;
  }
  return false;
}

// calculate where "display" screen falls under a calculator
char* computeDisplay(struct Calculator* calc, Camera2D camera) {
  if(!calc) return NULL;
  // Calculate display screen
  Vector2 dim = calc->corner;
  float unit = calc->unit;
  Rectangle displayScreen = {
    .x = dim.x+unit,
    .y = dim.y+unit,
    .width = calc->len - unit*2,
    .height = calc->len - unit*2
  };

  // set style
  GuiSetStyle(DEFAULT, TEXT_SIZE, calc->len/(strlen(calc->res)+1));
  if(calc->noval) GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0x686868ff);
  else GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0x121212ff);

  if(cameraGuiButton(displayScreen, calc->res, camera)) {
    // Tell dad.
    return strndupClone(calc->res, 128);
  }
  return NULL;
}

// compute and draw the display as well as children-buttons
void drawDisplay(struct Calculator* calc, Camera2D camera) {
  for(int i = 0; i < 9; i++) {
    char* res = computeDisplay(calc->buttons[i],camera);
    if(!res) continue;
    else if(strchr(res, 'X')) {
      replacex(res, calc->res, 128);
      strncpy(calc->res, res, 128);
    }
    else addStringToDisplay(res, calc);
    calc->noval = false;
    free(res);
  }
  calculate(calc);
  computeDisplay(calc, camera);
}

// compute and draw fixed buttons for a calculator
void drawFixedButtons(struct Calculator* calc, Camera2D camera) {
  // Imagine the calculator as a grid with gaplength of len.
  GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0x121212ff);
  GuiSetStyle(DEFAULT, TEXT_SIZE, calc->unit);
  fixedButton(calc, "X", 0, 0, camera);
  fixedOperator(calc, "+", 0, 4, camera);
  fixedOperator(calc, "-", 1, 4, camera);
  fixedOperator(calc, "^", 2, 4, camera);
  fixedOperator(calc, "*", 3, 4, camera);
  fixedOperator(calc, "/", 4, 4, camera);
  computeClear(calc, camera);
}

// Draw a calculator and all its children.
void drawCalculator(struct Calculator* calc, Camera2D camera) {
  if(!calc) return;
  if(!isVisible(calc, camera)) return; // optimize zoomed out invisible stuff away.

  // draw the stuff known ahead-of-time first
  Vector2 dim = calc->corner;
  DrawRectangle(dim.x, dim.y, calc->len, calc->len, LIGHTGRAY);
  drawFixedButtons(calc, camera);

  // if there are on buttons then we stop here.
  if(!calc->buttons) {
    calculate(calc);
    computeDisplay(calc, camera);
    birth(calc);
    return;
  }

  // otherwise we can compute the screen based on buttons
  drawDisplay(calc, camera);

  // now repeat these on the children
  for(int i = 0; i < 9; i++) {
    drawCalculator(calc->buttons[i], camera);
  }
}

#endif
