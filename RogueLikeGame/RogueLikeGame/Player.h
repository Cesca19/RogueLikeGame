#pragma once
#include "Character.h"

class Player : public Character {
public:
	Player(int x, int y);

	Vector2i GetPosition() const { return Position; }
	void SetPosition(Vector2i position) {
		Position = position;
	}

};

