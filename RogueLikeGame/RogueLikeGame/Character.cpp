#include "Character.h"

Character::Character(int Hp, int DamageAmount, char Skin) : _mHp(Hp), _mDamageAmount(DamageAmount), _mSkin(Skin)
{
}

Character::~Character()
{
}

void Character::ApplyDamage(std::shared_ptr<Character> Target, int DamageAmount)
{
	Target->TakeDamage(DamageAmount);
}

void Character::TakeDamage(int DamageAmount)
{
	_mHp -= DamageAmount;
	if (_mHp <= 0)
		OnDeath();
}
