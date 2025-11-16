#include "Player.h"
#include "Camera.h"
#include "config.h"

Player::Player(int x, int y) : Entity(x, y, 3) {

  dir = UP;

  vx = 0;
  vy = 0;
}

void Player::draw(ncplane *plane) {
  int sx = worldToScreenX(x);
  int sy = worldToScreenY(y);

  switch (dir) {
  case UP:
    ncplane_putstr_yx(plane, sy, sx, "▲");
    // ncplane_putstr_yx(plane, sy, sx - 1, "/ \\");
    break;
  case DOWN:
    // ncplane_putstr_yx(plane, sy, sx - 1, "\\ /");
    ncplane_putstr_yx(plane, sy, sx, "▼");
    break;
  case LEFT:
    ncplane_putstr_yx(plane, sy, sx, "◀");
    // ncplane_putstr_yx(plane, sy - 1, sx, "/");
    // ncplane_putstr_yx(plane, sy + 1, sx, "\\");
    break;
  case RIGHT:
    ncplane_putstr_yx(plane, sy, sx, "▶");
    // ncplane_putstr_yx(plane, sy - 1, sx, "\\");
    // ncplane_putstr_yx(plane, sy + 1, sx, "/");
    break;
  }
}

void Player::update() {
  x += vx;
  y += vy;

  // Boundary checking to prevent going off-screen
  if (x < 0)
    x = 0;
  if (x >= PLAY_AREA_WIDTH)
    x = PLAY_AREA_WIDTH;

  if (y < 0)
    y = 0;
  else if (y >= PLAY_AREA_HEIGHT)
    y = PLAY_AREA_HEIGHT;
}

void Player::move(int dx, int dy) {
  if (dx > 0 && dy == 0)
    dir = RIGHT;
  else if (dx < 0 && dy == 0)
    dir = LEFT;
  else if (dx == 0 && dy > 0)
    dir = DOWN;
  else if (dx == 0 && dy < 0)
    dir = UP;
  else if (dx < 0 && dy < 0)
    dir = UP_LEFT;
  else if (dx > 0 && dy < 0)
    dir = UP_RIGHT;
  else if (dx < 0 && dy > 0)
    dir = DOWN_LEFT;
  else if (dx > 0 && dy > 0)
    dir = DOWN_RIGHT;
}

void Player::setVelocity(int new_vx, int new_vy) {

  vx = new_vx;

  vy = new_vy;
}

int Player::getVX() const { return vx; }

int Player::getVY() const { return vy; }

Player::Direction Player::getDirection() const { return dir; }

void Player::setWord(char* w) {
    word = w;
}
