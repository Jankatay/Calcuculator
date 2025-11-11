#include "includes.h"
#include "base.h"
#include "calculator.h"

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
    .zoom = 0.50         // scaling.
  };
  // topmost calculator for you to zoom towards.
  float biggestLen = (GetScreenWidth() > GetScreenHeight()) ? GetScreenWidth() : GetScreenHeight();

  // in computer graphics top-left is (0,0). 
  // Positive Y means going down and positive X means going right from there.
  struct Calculator* topmost = initCalculator(0, 0, biggestLen);
  giveBirth(topmost);
  for(int i = 0; i < 9; i++) {
    giveBirth(topmost->buttons[i]);
  }

  // Raylib will basically run these functions every frame. 
  // It will be smooth as long as we calculate everything BEFORE drawing all at once.
  while(!WindowShouldClose()) {
    mainCamera = calculateCameraZoom(mainCamera);

    // Draw everything with respect to the camera.
    BeginDrawing();
    ClearBackground(WHITE);
    BeginMode2D(mainCamera); {
      // Usage and title
      DrawText("Usage: Try raising to 0 to\nget started. If you write\n something that includes \n'x' you can 'apply' it like\nan operator.", 0, 420, 30, BLACK);
      DrawText("Calcuculator. Inspired by lisp?", 0, -150, 30, RED);
      drawCalculator(topmost, &mainCamera);
    }EndMode2D();
    EndDrawing();
  }

  // free all allocated memory and exit.
  CloseWindow();
  return 0;
}
