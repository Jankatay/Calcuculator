#ifndef _CALCULATOR_H
#define _CALCULATOR_H

#include "includes.h"
#include "base.h"

// functions and structs to calculate user input and display on the calculator

// a tree
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
