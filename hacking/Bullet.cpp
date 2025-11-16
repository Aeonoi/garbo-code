#include "Bullet.h"
#include "config.h"
#include "Camera.h"
#include <random>

Bullet::Bullet(int x, int y, int new_vx, int new_vy) : Entity(x, y, 1) { // Bullets have 1 hp
    vx = new_vx;
    vy = new_vy;
}

void Bullet::draw(ncplane* plane) {
    const char* symbol;
    if (letter == '\0') 
        symbol = "\\0";
    else
        symbol = &letter;
    int sx = worldToScreenX(x);
    int sy = worldToScreenY(y);
    ncplane_set_fg_rgb8(plane, rand() % 255, rand() % 255, rand() % 255);

    // draw your arrow
    ncplane_putstr_yx(plane, sy, sx, symbol);

    // set color back to normal gray for other draws
    ncplane_set_fg_rgb8(plane, 200, 200, 200);
}

void Bullet::update() {
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
