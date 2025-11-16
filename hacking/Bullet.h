#pragma once

#include "Entity.h"

class Bullet : public Entity {
public:
    enum Direction { UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT };
    Bullet(int x, int y, int vx, int vy);

    void draw(ncplane* plane) override;
    void update() override;
    void setWord(char w) {
        letter = w;
    };
    int getx() const { return x; }
    int gety() const { return y; }
    int getDamage() const { return damage; }

private:
    int vx, vy; // velocity
    int damage = 1;
    char letter;
    int indx = 0;
};
