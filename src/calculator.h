#ifndef _CALCULATOR_H
#define _CALCULATOR_H

#include "includes.h"
#include "base.h"

// functions and structs to calculate user input and display on the calculator

// a tree
struct Calculator {
  struct Calculator** buttons;  // array of 9 little calculators.
  char res[32];                // result of whatever expressions you been inserting.
  Vector2 corner;               // top left corner
  float len;                    // width/height
  float unit;                   // width/height unit for each small button
  bool noval;                   // If next input should overwrite or not basically
};

// make a new calculator at a given corner with specific length.
struct Calculator* initCalculator(float xPos, float yPos, float len) {
  struct Calculator* calc = (struct Calculator*)calloc(1, sizeof(struct Calculator));
  
  // no value at startup
  calc->buttons = NULL;
  calc->res[0] = '0';
  calc->res[1] = 0;
  calc->noval = true;
  
  // main dimensions
  calc->corner = (Vector2){xPos, yPos};
  calc->len = len;
  calc->unit = len/5;
  return calc;
}

// add bunch of child nodes to make calculator more COMPLETE after initializing
void birth(struct Calculator* dad) {
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

// calculate result of symbols. No operator precedence because shunting-yard algorithm too powerful.
// err is 0 on fine, 1 on bad input, and 2 on 0/0
float calculateSymbols(char* symbols[32], int symlen, int* err) {
  if(!symbols[0]) {
    *err = 1;
    return 0;
  }
  // init
  float res = 0;
  char optr = '+';
  char* errStatus = NULL;
  bool invalid = false;
  int sign = 1;
  // for each symbol
  for(int i = 0; (!invalid) && i < symlen && symbols[i]; i++) {
    // get the symbol
    float next = strtof(symbols[i], &errStatus);
    if(strcmp(errStatus, ".") == 0) next = 0.0;
    
    // if not numeric set new operator
    if(errStatus && (strlen(errStatus) > 0)) {
      // ensure they are in proper order
      if(optr && errStatus[0] == '-') sign *= -1;
      else {
        invalid = errStatus[1] || optr;
        optr = *errStatus;
        sign = 1;
      }
      continue;
    }

    // else handle operators.
    if(optr == '+') res += next*sign; 
    else if(optr == '-') res -= next*sign; 
    else if(optr == '^') res = powf(res, next*sign); 
    else if(optr == '*') res *= next*sign; 
    else if(optr == '/' && next != 0) res /= next*sign; 

    // None of these means invalid. 
    else if(optr == '/' && res == 0) {
      *err = 2;
      return 0;
    }
    else invalid = true;
    optr = '\0';
  }
  invalid = optr;
  if(err || sign == -1) *err = invalid;
  return res;
} 

// calculate with no thought about operator precedence whatsoever. 
// replaces calc->res with result if valid.
void calculate(struct Calculator* calc) {
  // check everything is valid first.
  if(calc->noval) return;
  
  // Regex may look scary first time but MUCH better than coding all this manually
  // Will extract all the math symbols such that "9. ^-.12" -> {"9.", "^", "-.12"} 
  //static const char* mathSymbols = "([+-^*/])|(-?[0-9]+\\.?[0-9]?)|(-?\\.[0-9]+)";
  static const char* mathSymbols = "[+-^*/]|([0-9]+\\.?[0-9]?+)|(\\.[0-9]+)";
  char* symbols[32] = {0};
  int symlen = regexplit(calc->res, symbols, mathSymbols);
  if(symlen < 0) return;

  // now calculate each symbol in a loop
  int err = 0;
  float calculation = calculateSymbols(symbols, symlen, &err);
  
  // free the memory
  for(int i = 0; i < symlen; i++) {
    free(symbols[i]);
  }

  // invalid
  if(err == 1) return;
  if(err == 2) {
    strncpy(calc->res, "lim 0/0", 32);
    return;
  }

  // valid.
  strfromf(calc->res, 32, "%.2f", calculation);
}

// scale a calculator to be bigger/smaller for zooming
void resize(struct Calculator* calculator, float scale) {
  if(!calculator) return;
  // scale all numbers
  calculator->corner = Vector2Multiply(calculator->corner, (Vector2){scale, scale});;
  calculator->len *= scale;
  calculator->unit *= scale;
  // now do same for children
  if(!calculator->buttons) return;
  for(int i = 0; i < 9; i++) {
    resize(calculator->buttons[i], scale);
  }
}

// check if a calculator is currently visible or not.
bool isVisible(struct Calculator* calculator, Camera2D camera) {
  Vector2 calcCorner = GetWorldToScreen2D(calculator->corner, camera);

  // left top corner is at right or bottom of the visible screen
  if(calcCorner.x > GetScreenWidth()) return false;
  if(calcCorner.y > GetScreenHeight()) return false;

  // The length isn't enough to reach the screen.
  float len = calculator->len * camera.zoom;
  if(len < 100) return false; // zoomed out too much = invisible
  if((calcCorner.x + len) < 0) return false;
  if((calcCorner.y + len) < 0) return false;

  return true;
}

// recursively free a calculator tree.
void freeTree(struct Calculator** head) {
  struct Calculator* calculator = *head;
  // sanitize
  if(!calculator) return;
  if(!calculator->buttons) return;
  
  // free all children first.
  for(int i = 0; i < 9; i++) freeTree(&calculator->buttons[i]);
  free(calculator->buttons);

  // set it to NULL as good practice
  free(*head);
  *head = NULL;
}

#endif
