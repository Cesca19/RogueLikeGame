#pragma once

struct Vector2i {
	int x, y;
};

class Entity
{
public:
	Entity();
	~Entity();
	Vector2i GetPosition();
	void SetPosition(Vector2i Position);

protected:
	Vector2i _mPosition;
};

