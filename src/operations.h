#ifndef _OPERATIONS_H
#define _OPERATIONS_H

// Calculate operations. No operator precedence.
// Does not use regex so windows doesn't complain

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

// clone of linux's strndup so it runs on windows
char* strndupClone(const char* s, int n) {
  if(!s || n < 0) return NULL;
  // allocate the new memory
  int len = strnlen(s, n);
  char* cpy = (char*)calloc(len, sizeof(char));
  // copy old one and return.
  strncpy(cpy, s, len);
  return cpy;
}

/* -------------------------------------------------------------
| handle operation on a target "left." Returns true on success. |
|---------------------------------------------------------------|
| Ex. int left = 12;                                            |
|     handleOperation(&left, '+', 28);                          |
 -------------------------------------------------------------*/
bool handleOperation(float* left, char optr, float right) {
  if(!left) return false;

  // simply check if it's a correct operator or not.
  switch(optr) {
    // the 5 operators.
    case '^': 
      *left = powf(*left, right); 
      break;
    case '+': 
      *left += right; 
      break;
    case '-': 
      *left -= right; 
      break;
    case '*': 
      *left *= right; 
      break;
    case '/': 
      *left /= right; 
      break;
    // invalid char
    default: return false;
  }
  return true;
}

/* ------------------------------------------------------
| Calculate the string "str" of at most "maxlen" bytes.  |
| Will set "err" to the error status.                    |
|--------------------------------------------------------|
| EX. float res = calculate("5+12", 5, NULL);            |
|--------------------------------------------------------|
|--------------------------------------------------------|
| str    | immutable, null-terminated                    |
|--------|-----------------------------------------------|
| maxlen | -1 to auto-calculate length                   |
|--------|-----------------------------------------------|
| err    | mutable, nullable                             |
|        | None     => 0                                 |
|        | Invalid  => 1                                 |
|        | 0/0      => 2                                 |
 ------------------------------------------------------*/
float calculateOperations(const char* str, int maxlen, int* err) {
  if(!str) return 0;

  // init return value
  float left = 0;
  char optr = '+';
  int sign = 1;

  // for each element "index"
  char* index = (char*)str;
  for(int i = 0; i != maxlen && (*index); i++) {
    // skip whitespace
    if(isspace(*index)) {
      index++;
      continue;
    }

    // "plus" sign is fine but needs to be calculated.
    if(*index == '+') {
      // as a "positive sign" is invalid.
      if(err) *err = 1;
      if(optr) return 0;

      // as a "plus" is fine.
      if(err) *err = 0;
      optr = '+';
      index++;
      continue;
    }

    // Get first symbol and check if it's an operator
    char* tmp = index;            
    float right = strtof(index, &tmp);
    bool isNewOptr = (index == tmp);

    /* Handle operator type symbols. */
    // new operatot
    if(isNewOptr && !optr) {
      optr = *index;
      index++;
      continue;
    }

    // there was already an operator
    if(isNewOptr && optr) {
      // overwriting old operators is invalid. 
      if(err) *err = 1;
      if(*index != '-') return 0;
      // unless it was an innocent negative sign.
      if(err) *err = 0;
      sign *= 1;
      index++;
      continue;
    }     

    // ensure not dividing by 0
    if(*index == '/' && right == 0) {
      if(err) *err = (left == 0) ? 2 : 1;
      return 0;
    }
    
    // could handle the opration, reset the sign and continue.
    if(handleOperation(&left, optr, right*sign)) {
      // reset the sign and operator since valid operator
      sign = 1;
      optr = '\0';
      index = tmp;
      continue;
    }

    // something went wrong.
    if(err) *err = 1;
    return 0;
  }

  // sanitize and return.
  if(optr) {
    if(err) *err = 1;
    left = 0;
  }
  return left;
}

/* -------------------------------------------------------------
| Replace all capital 'X' in string "dst" with string "buf."    |
| "buf" is at most "buflen" bytes                               |
|---------------------------------------------------------------|
| Ex. char out[128] = "hello, X";                               |
|     replacex(dst, "world", 6);                                |
 -------------------------------------------------------------*/
void replacex(char dst[128], char* buf, int buflen) {
  if(!buf) return;

  // make a copy
  int bufsize = strnlen(buf, buflen);
  char* cpy = strndupClone(dst, 128);
  if(!cpy) return;
  dst[0] = '\0';

  // for each element in the copy
  int index = 0;
  for(int i = 0; (i < 128); i++) {
    // sanitize
    if(!cpy[i] || index > 128) break;
    // overwrite X
    if(cpy[i] == 'X') {
      strncat(dst, buf, 128-index);
      index += bufsize;
      continue;
    }
    // copy everything else as is.
    dst[index++] = cpy[i];
    dst[index] = '\0';
  }

  // free the copy
  free(cpy);
};

#endif
