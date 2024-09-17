#pragma once
#include "Monster.h"

class Faucheur : public Monster
{
public:
	Faucheur(int Hp, int DamageAmount, char Skin);
	~Faucheur();
	void Update(std::vector<std::shared_ptr<Character>> GameCharacters, std::vector<std::string> GameMap) override;
	void MoveTo(Vector2i TargetPosition) override;
	void Reward(std::vector<std::shared_ptr<Character>> GameCharacters) override;
	void OnDeath() override;
};

