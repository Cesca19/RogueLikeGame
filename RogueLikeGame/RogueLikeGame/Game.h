#pragma once
#include <memory>
#include <queue>
#include <string>
#include <vector>

#include "Character.h"
#include "Monster.h"
#include "Navigator.h"
#include "Player.h"

class Game {

public:
	Game();
	virtual ~Game();
	void Init();
	void LoadMap();
	void Run();
	void Render();
	void HandleInput();
	void Move();
	bool IsValidMove(int x, int y);
	void MoveNavigator(int dx, int dy);

private:
	std::shared_ptr<Player> _mPlayer = nullptr;
	std::shared_ptr<Navigator> _mNavigator = nullptr;
	std::vector<std::shared_ptr<Monster>> _mMonsters;
	std::vector<std::shared_ptr<Character>> _mCharacters;
	std::queue<std::string> _mCombatLog;
	std::vector<std::string> _mMap;

};

