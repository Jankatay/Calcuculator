#ifndef _CALCULATOR_H
#define _CALCULATOR_H

#include "includes.h"
#include "base.h"

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
    return true;
  }
  return false;
}

// add a string to a display
void addStringToDisplay(const char* str, struct Calculator* calc) {
  if(!calc || !str) return;
  // there was an input and screen was clear
  if(strncmp(calc->res, "0", 255) == 0) {
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
    computeDisplay(calc, camera);
    return;
  }

  // otherwise we can compute the screen based on buttons
  for(int i = 0; i < 9; i++) {
    char* res = computeDisplay(calc->buttons[i],camera);
    addStringToDisplay(res, calc);
  }
  computeDisplay(calc, camera);

  // now repeat these on the children
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
