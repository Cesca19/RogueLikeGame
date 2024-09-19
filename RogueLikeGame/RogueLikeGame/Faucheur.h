#pragma once
#include "Monster.h"

class Faucheur : public Monster
{
public:
	Faucheur(int Hp, int DamageAmount, char Skin);
	~Faucheur();
	void Update(std::vector<std::shared_ptr<Character>> GameCharacters, std::vector<std::string> GameMap) override;
	std::vector<Vector2i> MoveTo(Vector2i TargetPosition, std::vector<std::string> GameMap);
	void MoveTo(Vector2i TargetPosition) override;

	void Reward(std::vector<std::shared_ptr<Character>> GameCharacters) override;
	void OnDeath() override;
private:
	std::vector<Vector2i> GetReachablePosition(Vector2i TargetPosition, std::vector<std::string> GameMap);
	std::vector<Vector2i> GetPathTo(Vector2i TargetPosition, std::vector<std::string> GameMap);
	void FindNextMove(std::shared_ptr<Character> TargetPlayer, std::vector<std::string> GameMap);
	void Attack(std::shared_ptr<Character> TargetPlayer);

	Vector2i GetLeastF(std::unordered_map<Vector2i, std::pair<Vector2i, Vector2i>> map);
	std::vector<Vector2i> GetSuccessors(Vector2i parent, std::vector<std::string> GameMap);
	std::vector<Vector2i> GetPathway(Vector2i Dest, std::unordered_map<Vector2i, std::pair<Vector2i, Vector2i>> PrevPos);
};

