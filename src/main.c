#include "includes.h"
#include "base.h"
#include "calculator.h"
#include "draw.h"

// check if zoomed too far.
bool zoomFar(struct Calculator* calc, Camera2D screen);

// make a grandpa calculator and have it adopt this one
struct Calculator* adopt(struct Calculator* oldhead);

// readjust camera and calculator
struct Calculator* readjust(struct Calculator* calc, Camera2D* camera, float scale);

// I love raylib!
int main() {
  // initialize the graphics library.
  InitWindow(400, 400, "Calcuculator");
  float startPosition = 400;
  SetTargetFPS(60);

  // Raylib camera is pretty intuitive. It makes an infinite-dimension "screen" you can think of like desmos.
  // GetScreenToWorld() will calculate where something on your screen(like mouse) would be positioned in that world.
  // GetWorldToScreen() will calculate where something in the world would be in your screen.
  struct Camera2D mainCamera = {
    .offset = {0, 0},         // displacement from target.
    .target = {-200, -200},   // rotation and zoom origin.
    .rotation = 0,            // in degrees :3
    .zoom = 0.2               // scaling.
  };

  // in computer graphics top-left is (0,0). 
  // Positive Y means going down and positive X means going right from there.
  struct Calculator* topmost = initCalculator(0, 0, startPosition);
  birth(topmost);
  topmost = adopt(topmost);

  // Raylib will basically run these functions every frame. 
  // It will be smooth as long as we calculate everything BEFORE drawing all at once.
  while(!WindowShouldClose()) {
    // calculateCameraZoom reaches floating-point bit-limit FAST so resize everything and offset camera once in a while to give illusion of zoom instead.
    while(zoomFar(topmost, mainCamera) || mainCamera.zoom < 0.0016) {
      topmost = readjust(topmost, &mainCamera, 0.0016);
    }
    while(mainCamera.zoom > 4) {
      topmost = readjust(topmost, &mainCamera, 25.0);
    }
    mainCamera = calculateCameraZoom(mainCamera);

    // Draw everything with respect to the camera.
    ClearBackground(BLACK);
    BeginDrawing();
    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
    GuiDrawText("You just lost the game :3", (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()}, TEXT_ALIGN_CENTER, RED);
    // Draws from most visible to least visible in order so it's still fine to calculate as I`` draw in this case.
    BeginMode2D(mainCamera); {
      drawCalculator(topmost, mainCamera);
    }EndMode2D();
    EndDrawing();
  }

  // free all allocated memory and exit
  freeTree(&topmost);
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
struct Calculator* adopt(struct Calculator* oldhead) {
  float oldlen = oldhead->len;
  
  // make a grandpa node with missing leftTop corner
  struct Calculator* newhead = initCalculator(oldhead->corner.x - 9*oldlen, oldhead->corner.y - 3*oldlen, oldhead->len * 25.0);
  birth(newhead);
  for(int i = 0; i < 9; i++) birth(newhead->buttons[i]);

  // set rightBottom corner of leftTop corner to be old head
  struct Calculator* newpos = newhead->buttons[3]->buttons[8];
  memcpy(newpos, oldhead, sizeof *oldhead);
  free(oldhead);
  return newhead;
}

// resize a calculator and offset camera to still be around same.
struct Calculator* readjust(struct Calculator* calc, Camera2D* camera, float scale) {
  // resize calculator
  calc = adopt(calc);
  resize(calc, scale);
  // set camera.
  camera->zoom /= scale;
  camera->target.x *= scale;
  camera->target.y *= scale;
  return calc;
}
