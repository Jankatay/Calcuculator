// Base functions for zooming and drawing etc.

#ifndef _BASE_H
#define _BASE_H

#include "includes.h"
#include "operations.h"

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
int cameraGuiButton(Rectangle bounds, const char *text, Camera2D camera) {
    int result = 0;
    GuiState state = guiState;

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiControlExclusiveMode) {
        // this was literally the only line I had to modify
        //Vector2 mousePoint = GetMousePosition(); 
        Vector2 mousePoint = GetScreenToWorld2D(GetMousePosition(), camera);     
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

#endif
