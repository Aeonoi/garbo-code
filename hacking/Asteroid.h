#pragma once

#include "Entity.h"
#include <notcurses/notcurses.h>

class Asteroid : public Entity {
public:
    Asteroid(int x, int y, int size);

    void draw(ncplane* plane) override;
    void update() override;
    int getx() const { return x; }
    int gety() const { return y; }
    void damage(int dmg) { 
        hp -= dmg; 
    }
    int gethp() const { return hp; }

private:
    int size;
    double vx, vy; // velocity
};
