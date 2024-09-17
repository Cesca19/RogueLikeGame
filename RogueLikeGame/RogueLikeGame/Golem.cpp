#include "Golem.h"

Golem::Golem(int Hp, int DamageAmount, char Skin, int DamageIncreaseAmount) : 
	Monster(Hp, DamageAmount, Skin), _mResistanceWeight(40), _mDamageIncreaseAmount(DamageIncreaseAmount)
{
}

Golem::~Golem()
{
}

void Golem::Update(std::vector<std::shared_ptr<Character>> GameCharacters, std::vector<std::string> GameMap)
{
}

void Golem::MoveTo(Vector2i TargetPosition)
{
}

void Golem::Reward(std::vector<std::shared_ptr<Character>> GameCharacters)
{
	for (int i = 0; i < GameCharacters.size(); i++) {
		std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(GameCharacters[i]);
		if (player != nullptr)
			player->InreaseDamage(_mDamageIncreaseAmount);
	}
}

void Golem::TakeDamage(int Amount)
{
	bool canResit = ((rand() %  101) <= _mResistanceWeight) ? true : false;
	if (canResit)
		_mResistanceWeight -= 10;
	else {
		_mResistanceWeight += 10;
		_mHp -= Amount;
		if (_mHp <= 0)
			OnDeath();
	}
	if (_mResistanceWeight < 10 || _mResistanceWeight >= 90)
		_mResistanceWeight = 40;
}

void Golem::OnDeath()
{
}
