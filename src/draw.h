#ifndef _DRAW_H
#define _DRAW_H

#include "includes.h"
#include "base.h"
#include "calculator.h"

// calculate where "clear" button falls under a calculator
bool computeClear(struct Calculator* calc, Camera2D* camera) {
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
    strncpy(calc->res, str, 255);
    return;
  }
  // there was an input and screen had stuff on it.
  int distance = 255 - strnlen(str, 255);
  strncat(calc->res, str, distance);
}

// calculate where "label" button falls under a calculator
bool fixedButton(struct Calculator* calc, const char* label, float xPos, float yPos, Camera2D* camera) {
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
bool fixedOperator(struct Calculator* calc, const char* label, float xPos, float yPos, Camera2D* camera) {
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
    float len = strnlen(label, 255);
    strncat(calc->res, label, 255-len);
    calc->noval = false;
    return true;
  }
  return false;
}

// calculate where "display" screen falls under a calculator
char* computeDisplay(struct Calculator* calc, Camera2D* camera) {
  if(!calc || !camera) return NULL;
  // Calculate display screen
  Vector2 dim = calc->corner;
  float unit = calc->unit;
  Rectangle displayScreen = {
    .x = dim.x+unit,
    .y = dim.y+unit,
    .width = calc->len - unit*2,
    .height = calc->len - unit*2
  };
  // compute the button
  if(cameraGuiButton(displayScreen, calc->res, camera)) {
    // Tell dad.
    calc->noval = false;
    return strndup(calc->res, 255);
  }
  return NULL;
}

// Draw a calculator and all its children.
void drawCalculator(struct Calculator* calc, Camera2D* camera) {
  if(!calc) return;
  // dimensions
  Vector2 dim = calc->corner;
  GuiSetStyle(DEFAULT, TEXT_SIZE, calc->unit);

  // first the background colors
  DrawRectangle(dim.x, dim.y, calc->len, calc->len, LIGHTGRAY);

  // Compute the buttons.
  // Image the calculator as a grid with gaplength of long.
  fixedButton(calc, "X", 0, 0, camera);
  fixedOperator(calc, "+", 0, 4, camera);
  fixedOperator(calc, "-", 1, 4, camera);
  fixedOperator(calc, "^", 2, 4, camera);
  fixedOperator(calc, "*", 3, 4, camera);
  fixedOperator(calc, "/", 4, 4, camera);
  computeClear(calc, camera);

  // if there are on buttons then we stop here.
  if(!calc->buttons) {
    calculate(calc);
    computeDisplay(calc, camera);
    return;
  }

  // otherwise we can compute the screen based on buttons
  for(int i = 0; i < 9; i++) {
    char* res = computeDisplay(calc->buttons[i],camera);
    addStringToDisplay(res, calc);
  }
  calculate(calc);
  computeDisplay(calc, camera);

  // now repeat these on the children
  for(int i = 0; i < 9; i++) {
    drawCalculator(calc->buttons[i], camera);
  }
}

#endif
