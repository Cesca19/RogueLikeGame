#pragma once
#include <string>

struct Vector2i {
	int x, y;

	bool operator==(const Vector2i& other) const {
		return x == other.x && y == other.y;
	}
	bool operator!=(const Vector2i& other) const {
		return !(*this == other);
	}
	bool operator<(const Vector2i& other) const {
		return x == other.x && y == other.y;
	}

	Vector2i& operator=(const Vector2i& other) {
		x = other.x, y = other.y;
		return *this;
	}
};

namespace std {
	template <>
	struct hash<Vector2i> {
		std::size_t operator()(const Vector2i& v) const noexcept {
			return std::hash<int>()(v.x) ^ std::hash<int>()(v.y);
		}
	};
}

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

