#pragma once

#include <utility>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include "Character.h"


#define RESET   "\033[0m"

class Monster : public Character
{
public:
	Monster(int Hp, int DamageAmount, char Skin);
	~Monster();
	virtual void Update(std::vector<std::shared_ptr<Character>> GameCharacters, std::vector<std::string> GameMap) = 0;
	virtual void Reward(std::vector<std::shared_ptr<Character>> GameCharacters) = 0;
	virtual bool IsPlayerClose(std::shared_ptr<Character> TargetPlayer);
	void MoveTo(Vector2i TargetPosition) override;
protected:
	// A* path finding
	Vector2i GetLeastF(std::unordered_map<Vector2i, std::pair<Vector2i, Vector2i>> map);
	std::vector<Vector2i> GetSuccessors(Vector2i parent, std::vector<std::string> GameMap);
	std::vector<Vector2i> GetPathway(Vector2i Dest, std::unordered_map<Vector2i, std::pair<Vector2i, Vector2i>> PrevPos);
	std::vector<Vector2i> MoveTo(Vector2i TargetPosition, std::vector<std::string> GameMap);
	std::vector<Vector2i> GetPathTo(Vector2i TargetPosition, std::vector<std::string> GameMap);

protected:
	int _mMoveLimit;
};

