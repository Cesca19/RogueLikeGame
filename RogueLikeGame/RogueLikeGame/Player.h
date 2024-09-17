#pragma once

#include "Character.h"

class Player : public Character
{
public:
	Player(int Hp, int DamageAmount, char Skin);
	~Player();
	void Update(std::vector<std::shared_ptr<Character>> GameCharacters, std::vector<std::string> GameMap) override;
	void MoveTo(Vector2i TargetPosition) override;
	void InreaseDamage(int Amount);
	void RestoreHp();
	void OnDeath() override;
private:
	int _mMaxHp;
};

