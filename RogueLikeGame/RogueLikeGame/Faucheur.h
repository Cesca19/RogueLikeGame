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
private:
	std::vector<Vector2i> GetReachablePosition(Vector2i TargetPosition, std::vector<std::string> GameMap);
	std::vector<Vector2i> GetPathTo(Vector2i TargetPosition);
	void FindNextMove(std::shared_ptr<Character> TargetPlayer, std::vector<std::string> GameMap);
	void Attack(std::shared_ptr<Character> TargetPlayer);
	bool IsPlayerClose(std::shared_ptr<Character> TargetPlayer);
};

