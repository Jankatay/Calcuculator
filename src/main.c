#include "includes.h"
#include "base.h"
#include "calculator.h"
#include "draw.h"

// check if zoomed too far.
bool zoomFar(struct Calculator* calc, Camera2D screen);

// make a grandpa calculator and have it adopt this one
struct Calculator* adopt(struct Calculator* oldhead);

// readjust camera and calculator
void readjust(struct Calculator* calc, Camera2D* camera, float scale);

// when zoomed in TOO much it will give a warning from among this array
const char* const messageArr[] = {"hello", "uwu", ":3", "zooming all by yourself handsome?", "zooming aww by youwsewf?", 
  "maybe later", "LMAO", "I refuse!", "you just lost the game", "why are you zooming?", "nah", "\"screw you\" *unzooms*", 
  "notice me ignore consent, microsoft", "maybe later", "NYI : not yet implemented.", "Check linux \"strings\" command"};
const size_t messageLen = sizeof(messageArr)/sizeof(char*);

// will have to edit this so windows doesn't lose its mind.
int main() {
  // initialize the graphics library.
  InitWindow(400, 400, "Calcuculator");
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  GuiSetStyle(DEFAULT, TEXT_SPACING, 10);
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

  // where in messages we are.
  int messageIndex = 0;

  // Raylib will basically run these functions every frame. 
  // It will be smooth as long as we calculate everything BEFORE drawing all at once.
  while(!WindowShouldClose()) {
    // set new zoom
    struct Camera2D temp = mainCamera;
    mainCamera = calculateCameraZoom(mainCamera);

    // ensure zoom is not TOO much.
    bool showFunnyMessage = false;
    if(fabs(topmost->corner.x) > 1000000000 || fabs(topmost->corner.y) > 1000000000) {
      mainCamera = temp;
      mainCamera.zoom /= 1.1;
      showFunnyMessage = true;
    } else {
      // poor mans' random number generator
      messageIndex = (messageIndex+1) % messageLen;
      showFunnyMessage = false;
    }

    // calculateCameraZoom reaches floating-point bit-limit FAST so resize everything and offset camera once in a while to give illusion of zoom instead.
    while(zoomFar(topmost, mainCamera)) {
      topmost = adopt(topmost);
    }
    while(mainCamera.zoom < 0.0016) {
      readjust(topmost, &mainCamera, 0.0016);
    }
    while(mainCamera.zoom > 4) {
      readjust(topmost, &mainCamera, 25.0);
    }

    // Draw everything with respect to the camera.
    ClearBackground(BLACK);
    BeginDrawing();
    // Draws from most visible to least visible in order so it's still fine to calculate as I`` draw in this case.
    BeginMode2D(mainCamera); {
      drawCalculator(topmost, mainCamera);
    }EndMode2D();
    GuiSetStyle(DEFAULT, TEXT_SIZE, 25);
    GuiSetStyle(DEFAULT, TEXT_WRAP_MODE, 1);
    if(showFunnyMessage) {
      GuiDrawText(messageArr[messageIndex], (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()}, TEXT_ALIGN_CENTER, RED);
    }
    GuiSetStyle(DEFAULT, TEXT_WRAP_MODE, 0);
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
void readjust(struct Calculator* calc, Camera2D* camera, float scale) {
  // resize calculator
  resize(calc, scale);
  // set camera.
  camera->zoom /= scale;
  camera->target.x *= scale;
  camera->target.y *= scale;
}
