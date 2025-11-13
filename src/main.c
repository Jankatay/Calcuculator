#include "includes.h"
#include "base.h"
#include "calculator.h"
#include "draw.h"

// check if zoomed too far.
bool zoomFar(struct Calculator* calc, Camera2D screen);
// make a grandpa calculator and have it adopt this one
struct Calculator* adoptCalculator(struct Calculator* oldhead);

// I love raylib!
int main() {
  // initialize the graphics library.
  InitWindow(400, 400, "Calcuculator");
  SetTargetFPS(60);

  // Raylib camera is pretty intuitive. It makes an infinite-dimension "screen" you can think of like desmos.
  // GetScreenToWorld() will calculate where something on your screen(like mouse) would be positioned in that world.
  // GetWorldToScreen() will calculate where something in the world would be in your screen.
  struct Camera2D mainCamera = {
    .offset = {0, 0},   // displacement from target.
    .target = {-200, -200},   // rotation and zoom origin.
    .rotation = 0,      // in degrees :3
    .zoom = 0.20         // scaling.
  };
  // topmost calculator for you to zoom towards.
  float biggestLen = 1600;

  // in computer graphics top-left is (0,0). 
  // Positive Y means going down and positive X means going right from there.
  struct Calculator* topmost = initCalculator(0, 0, biggestLen);
  giveBirth(topmost);
  for(int i = 0; i < 9; i++) {
    giveBirth(topmost->buttons[i]);
    for(int j = 0; j < 9; j++) {
      giveBirth(topmost->buttons[i]->buttons[j]);
    }
  }

  // Raylib will basically run these functions every frame. 
  // It will be smooth as long as we calculate everything BEFORE drawing all at once.
  while(!WindowShouldClose()) {
    mainCamera = calculateCameraZoom(mainCamera);
    if(zoomFar(topmost, mainCamera)) {
      topmost = adoptCalculator(topmost);
    }

    // Draw everything with respect to the camera.
    BeginDrawing();
    ClearBackground(WHITE);
    BeginMode2D(mainCamera); {
      // first the background colors
      DrawRectangle(topmost->corner.x, topmost->corner.y, topmost->len, topmost->len, LIGHTGRAY);
      drawCalculator(topmost, &mainCamera);
    }EndMode2D();
    EndDrawing();
  }

  // free all allocated memory and exit
  CloseWindow();
  return 0;
}

// check if camera is zoomed out the border of a calculator
bool zoomFar(struct Calculator* calc, Camera2D screen) { 
  // get the dimensions
  Vector2 leftTop = calc->corner;
  Vector2 rightBottom = {
    .x = calc->corner.x + calc->len,
    .y = calc->corner.y + calc->len
  };
  Vector2 screenMin = GetScreenToWorld2D((Vector2){0, 0}, screen);
  Vector2 screenMax = GetScreenToWorld2D((Vector2){GetScreenWidth(), GetScreenHeight()}, screen);
  // check if any are outside the screen.
  return (leftTop.x > screenMin.x) || (leftTop.y > screenMin.y) || (rightBottom.x < screenMax.x) || (rightBottom.y < screenMax.y);
}

// Replace head with a bigger new Calculator such that the old head is in there few children down.
struct Calculator* adoptCalculator(struct Calculator* oldhead) {
  float oldlen = oldhead->len;
  
  // make a grandpa node with missing leftTop corner
  struct Calculator* newhead = initCalculator(oldhead->corner.x - 9*oldlen, oldhead->corner.y - 3*oldlen, oldhead->len * 25);
  giveBirth(newhead);
  for(int i = 0; i < 9; i++) giveBirth(newhead->buttons[i]);

  // set rightBottom corner of leftTop corner to be old head
  struct Calculator* newpos = newhead->buttons[3]->buttons[8];
  memcpy(newpos, oldhead, sizeof *oldhead);
  free(oldhead);
  return newhead;
}
