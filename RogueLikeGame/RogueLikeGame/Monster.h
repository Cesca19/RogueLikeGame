#pragma once

#include <utility>
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
	virtual void MoveTo(Vector2i TargetPosition) = 0;
	virtual void Reward(std::vector<std::shared_ptr<Character>> GameCharacters) = 0;
	virtual bool IsPlayerClose(std::shared_ptr<Character> TargetPlayer);
protected:
	int _mMoveLimit;
};

