#include "Navigator.h"

Navigator::Navigator(int x, int y, char symbol) {
    _mPosition.x = x;
    _mPosition.y = y;
    _mSymbol = symbol;
}

void Navigator::Move(int dx, int dy) {
    _mPosition.x += dx;
    _mPosition.y += dy;
}
