#pragma once

#include "Entity.h"

class Player : public Entity {
public:
    enum Direction { UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT };

    Player(int x, int y);

    void draw(ncplane* plane) override;
    void update() override;
    void move(int dx, int dy);
    void setWord(char* w);
    void setVelocity(int vx, int vy);
    int getVX() const;
    int getVY() const;
    Direction getDirection() const;

private:
    Direction dir;
    int vx, vy;
    char * word;
};
