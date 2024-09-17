#pragma once
#include "Entity.h"

class Navigator : public Entity {
public:
    Navigator(int x, int y, char symbol);
    void Move(int dx, int dy);
};
