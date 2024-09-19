#include "Character.h"

Character::Character(int Hp, int DamageAmount, char Skin) : _mHp(Hp), _mDamageAmount(DamageAmount)
{
	_mSymbol = Skin;
	_mMaxHp = _mHp;
}

Character::~Character()
{
}

/*void Character::ApplyDamage(std::shared_ptr<Character> Target, int DamageAmount)
{
	Target->TakeDamage(DamageAmount);
}*/

void Character::TakeDamage(int DamageAmount, Character* Source)
{
	_mHp -= DamageAmount;
	if (_mHp <= 0)
		OnDeath();
}

void Character::SetGame(void* CurrentGame)
{
	_mGame = CurrentGame;
}
