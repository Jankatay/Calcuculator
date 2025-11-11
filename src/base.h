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
  if(camera.zoom <= 0.5) camera.zoom = 0.5;
  return camera;
}


#endif
