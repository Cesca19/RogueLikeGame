#include "Entity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
}

Vector2i Entity::GetPosition()
{
	return _mPosition;
}

void Entity::SetPosition(Vector2i Position)
{
	_mPosition = Position;
}
