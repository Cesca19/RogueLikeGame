#pragma once

struct Vector2i {
	int x, y;
};

class Entity {
public:
	Vector2i GetPosition() const { return _mPosition; }
	int GetX() const { return _mPosition.x; }
	int GetY() const { return _mPosition.y; }
	char GetSymbol() const { return _mSymbol; }

	void SetPosition(const Vector2i newPosition) {
		_mPosition = newPosition;
	}

protected:
	Vector2i _mPosition;
	char _mSymbol;


};

