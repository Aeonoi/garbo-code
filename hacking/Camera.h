#pragma once

class Camera {
public:
  Camera(int offsetX, int offsetY);
  int getOffsetX() const { return offsetX; }
  int getOffsetY() const { return offsetY; }
  void setOffset(int x, int y) {
    offsetX = x;
    offsetY = y;
  }

private:
  int offsetX, offsetY;
};
// Simple camera globals and helpers
#pragma once

extern int camX;
extern int camY;

static inline int worldToScreenX(int wx) { return wx - camX; }
static inline int worldToScreenY(int wy) { return wy - camY; }
