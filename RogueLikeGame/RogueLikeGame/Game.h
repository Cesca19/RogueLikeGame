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
	void Run();
	void AddToActionLog(const std::string& action);
	std::vector<std::shared_ptr<Character>> GetAllMonsters();
	std::shared_ptr<Player> GetPlayer();

private:
	void LoadMap();
	void Render();
	std::vector<std::string> RenderStats();
	void HandleInput();
	void Move();
	bool IsValidMove(Vector2i position);
	void MoveNavigator(int dx, int dy);

	std::vector<std::shared_ptr<Monster>> GetAttackableMonsters();
	void EnterAttackMode();
	void ExitAttackMode();
	void SelectNextMonster(int direction);
	void AttackMonster(std::vector<std::shared_ptr<Monster>>::const_reference monster);
	void PerformAttack();
	

public:
	std::vector<std::string> UpdateCharacterPositionInMap(Character* Target, Vector2i PrevPosition);

private:
	std::shared_ptr<Player> _mPlayer = nullptr;
	std::shared_ptr<Navigator> _mNavigator = nullptr;
	std::vector<std::shared_ptr<Monster>> _mMonsters;
	std::vector<std::shared_ptr<Character>> _mGameMonsters;
	std::vector<std::shared_ptr<Character>> _mCharacters;
	std::queue<std::string> _mCombatLog;
	std::vector<std::string> _mMap;
	std::deque<std::string> _mActionLog;
	const size_t MAX_LOG_ENTRIES = 8;

	int _mTurn;
	char _mAttackSymbol = '*';

	enum class GameState { Moving, Attacking };
	GameState _mCurrentState = GameState::Moving;
	std::vector<std::shared_ptr<Monster>> _mAttackableMonsters;
	size_t _mSelectedMonsterIndex = 0;
};

