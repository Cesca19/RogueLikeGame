#include "Monster.h"

Monster::Monster(int Hp, int DamageAmount, char Skin) : Character(Hp, DamageAmount, Skin), _mMoveLimit(10)
{
}

Monster::~Monster()
{
}