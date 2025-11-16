#include "Asteroid.h"
#include "Camera.h"
#include "config.h"
#include <cstdlib> // For rand()

Asteroid::Asteroid(int x, int y, int s) : Entity(x, y, s), size(s) {
  // Random velocity
  int min_speed, max_speed;
  if (size <= 2) { // Small asteroid
    min_speed = SMALL_ASTEROID_MIN_SPEED;
    max_speed = SMALL_ASTEROID_MAX_SPEED;
  } else { // Big asteroid
    min_speed = BIG_ASTEROID_MIN_SPEED;
    max_speed = BIG_ASTEROID_MAX_SPEED;
  }

  int speed = min_speed + (rand() % (max_speed - min_speed + 1));

  vx = (rand() % (2 * speed + 1)) - speed;
  vy = (rand() % (2 * speed + 1)) - speed;

  if (vx == 0 && vy == 0) {
    vx = 1; // Ensure it moves
  }
}

void Asteroid::draw(ncplane *plane) {
  // Draw a rough circle for the asteroid
  int sx = worldToScreenX(x);
  int sy = worldToScreenY(y);

  if (size == 1) {
    ncplane_putstr_yx(plane, sy, sx, "o");
  } else if (size == 2) {
    ncplane_putstr_yx(plane, sy, sx, "O");
  } else {
    ncplane_putstr_yx(plane, sy - 1, sx, ".-.");
    ncplane_putstr_yx(plane, sy, sx - 1, "( O )");
    ncplane_putstr_yx(plane, sy + 1, sx, "'-'");
  }
}

void Asteroid::update() {
  x += vx;
  if (x <= 0)
    x = 0;
  if (x >= PLAY_AREA_HEIGHT)
    x = PLAY_AREA_HEIGHT;

  y += vy;
  if (y <= 0)
    y = 0;
  if (y >= PLAY_AREA_WIDTH)
    y = PLAY_AREA_WIDTH;

  if (x <= 0 || x >= PLAY_AREA_WIDTH)
    vx = -vx; // Simple wall bouncing logic (needs plane dimensions)
  if (y <= 0 || y >= PLAY_AREA_HEIGHT)
    vy = -vy; // Simple wall bouncing logic (needs plane dimensions)
}
