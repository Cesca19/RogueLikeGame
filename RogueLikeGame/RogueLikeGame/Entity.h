#pragma once
#include <string>

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

	void SetColor(const std::string& color) { _mColor = color; }
	const std::string& GetColor() const { return _mColor; }

protected:
	Vector2i _mPosition;
	char _mSymbol;
	std::string _mColor;
};

