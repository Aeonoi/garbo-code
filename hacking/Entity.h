#pragma once

#include <notcurses/notcurses.h>

class Entity {
public:
  Entity(int x, int y, int hp);
  virtual ~Entity() = default;

  virtual void
  draw(ncplane *plane) = 0; // Pure virtual function for drawing
  virtual void update() = 0;    // Pure virtual for updating state

  int getX() const { return x; }
  int getY() const { return y; }
  int getHp() const { return hp; }
  void setPosition(int newX, int newY) { x = newX; y = newY; }
  void damage(int dmg) { hp -= dmg; }

protected:
  int x, y;
  int hp;
};
