#include "Monster.h"

Monster::Monster(int Hp, int DamageAmount, char Skin) : Character(Hp, DamageAmount, Skin), _mMoveLimit(5)
{
}

Monster::~Monster()
{
}

bool Monster::IsPlayerClose(std::shared_ptr<Character> TargetPlayer)
{
	Vector2i TargetPosition = TargetPlayer->GetPosition();
	std::vector<Vector2i> availablePositions = { {TargetPosition.x - 1, TargetPosition.y }, {TargetPosition.x + 1, TargetPosition.y},
												{ TargetPosition.x, TargetPosition.y - 1}, {TargetPosition.x, TargetPosition.y + 1} };

	for (int i = 0; i < availablePositions.size(); i++)
		if (_mPosition.y == availablePositions[i].y && _mPosition.x == availablePositions[i].x)
			return true;
	return false;
}
