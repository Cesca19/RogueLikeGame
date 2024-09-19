#pragma once

#include "Monster.h"
#include "Player.h"

class Golem : public Monster
{
public:
	Golem(int Hp, int DamageAmount, char Skin, int DamageIncreaseAmount, int HpIncreaseAmount);
	~Golem();
	void Update(std::vector<std::shared_ptr<Character>> GameCharacters, std::vector<std::string> GameMap) override;
	void MoveTo(Vector2i TargetPosition) override;
	void Reward(std::vector<std::shared_ptr<Character>> GameCharacters) override;
	void TakeDamage(int amount, Character *source) override;
	void OnDeath() override;
private:
	int _mResistanceWeight;
	int _mDamageIncreaseAmount;
	int _mHpIncreaseAmount;
};

