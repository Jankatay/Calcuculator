// Base functions for zooming and drawing etc.

#ifndef _BASE_H
#define _BASE_H

#include "includes.h"

// Calculate where camera will zoom based on mouse.
static Camera2D calculateCameraZoom(Camera2D camera) {
  float wheel = GetMouseWheelMove();
  if(wheel == 0) return camera;
  Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
  camera.offset = GetMousePosition();
  camera.target = mousePos;
  float scale = 0.2*wheel;
  camera.zoom = expf(logf(camera.zoom)+scale);
  // hard limit. Fonts mess up otherwise. Also floating point is still limited.
  return camera;
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

// split a string into symbols using extended regex.
// "[+-^*/]|-?[0-9]|"
int regexplit(char in[255], char* out[255], const char* re) {
  // Ensure valid regex
  regex_t regex;
  if(regcomp(&regex, re, REG_EXTENDED)) return 0;

  // use a pointer as index.
  char* index = in;

  int buflen = 0;
  // for each match (element in the produced list above)
  regmatch_t pmatch;
  for(int i = 0; regexec(&regex, index, 1, &pmatch, 0) == 0; i++) {
    // get the matched part
    regoff_t len = pmatch.rm_eo - pmatch.rm_so;
    char* res = strndup(index + pmatch.rm_so, len);
    // copy it over to buf
    out[buflen++] = res;
    index += pmatch.rm_eo;
  }
  regfree(&regex);
  return buflen;
}

// replace all matching regex with buf
void regexplace(char dst[255], char* buf, const char* re) {
  // ensure valid regex
  regex_t regex;
  if(regcomp(&regex, re, REG_EXTENDED)) return;

  // prepare dst
  char* str = strndup(dst, 255);
  dst[0] = '\0';
  int limit = 255;

  // for each matching regex
  regmatch_t pmatch;
  for(int i = 0; regexec(&regex, str, 1, &pmatch, 0) == 0; i++) {
    // sanitize
    if(limit <= 0) {
      dst[0] = '\0';
      break;
    }
    // append previous and buf
    strncat(dst, str, pmatch.rm_so);
    limit -= pmatch.rm_so;
    strncat(dst, buf, limit);
    str += pmatch.rm_eo;
  }
  // append whatever is left.
  strncat(dst, str, limit);

  // end
  dst[254] = '\0';
  regfree(&regex);
}

#endif
