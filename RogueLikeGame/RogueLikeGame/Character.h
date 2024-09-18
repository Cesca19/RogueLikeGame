#pragma once

#include "Entity.h"

#include <vector>
#include <memory>
#include <conio.h>
#include <string>
#include <cstdlib> 
#include <iostream> 

class Character : public Entity
{
public:
	Character(int Hp, int DamageAmount, char Skin);
	~Character();
	virtual void Update(std::vector<std::shared_ptr<Character>> GameCharacters, std::vector<std::string> GameMap) = 0;
	virtual void MoveTo(Vector2i TargetPosition) = 0;
	virtual void ApplyDamage(std::shared_ptr<Character> Target, int DamageAmount);
	virtual void TakeDamage(int DamageAmount);
	virtual void OnDeath() = 0;

	void SetGame(void* CurrentGame);
protected:
	int _mHp;
	int _mDamageAmount;
	void* _mGame;
	//char _mSkin;
};

