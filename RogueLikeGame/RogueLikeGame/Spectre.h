#pragma once

#include "Monster.h"
#include "Player.h"

class Spectre : public Monster
{
public:
	Spectre(int Hp, int DamageAmount, char Skin);
	~Spectre();
	void Update(std::vector<std::shared_ptr<Character>> GameCharacters, std::vector<std::string> GameMap) override;
	void MoveTo(Vector2i TargetPosition) override;
	void Reward(std::vector<std::shared_ptr<Character>> GameCharacters) override;
	Vector2i FleeFrom(std::shared_ptr<Character> Target, bool &CanFlee);
};

