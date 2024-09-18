#include "Faucheur.h"
#include "Game.h"

Faucheur::Faucheur(int Hp, int DamageAmount, char Skin) : Monster(Hp, DamageAmount, Skin)
{
}

Faucheur::~Faucheur()
{
}

void Faucheur::Update(std::vector<std::shared_ptr<Character>> GameCharacters, std::vector<std::string> GameMap)
{
	std::shared_ptr<Player> player = nullptr;
	Vector2i targetPosition;
	for (int i = 0; i < GameCharacters.size(); i++) {
		player = std::dynamic_pointer_cast<Player>(GameCharacters[i]);
		if (player != nullptr)
			break;
	}
	if (player != nullptr) {
		FindNextMove(player, GameMap);
		Attack(player);
	}
}



void Faucheur::MoveTo(Vector2i TargetPosition)
{
	Game* game = static_cast<Game*>(_mGame);
	int remaningMoves = _mMoveLimit;
	if (game == nullptr)
		exit(84);

	//replace with bfs or a*
	for (; _mPosition.x < TargetPosition.x && remaningMoves > 0; remaningMoves--) {
		_mPosition.x++;
		game->UpdateCharacterPositionInMap(this, Vector2i{ _mPosition.x - 1, _mPosition.y });
	}
	for (; _mPosition.x > TargetPosition.x && remaningMoves > 0; remaningMoves--) {
		_mPosition.x--;
		game->UpdateCharacterPositionInMap(this, Vector2i{ _mPosition.x + 1, _mPosition.y });
	}

	for (; _mPosition.y < TargetPosition.y && remaningMoves > 0; remaningMoves--) {
		_mPosition.y++;
		game->UpdateCharacterPositionInMap(this, Vector2i{ _mPosition.x, _mPosition.y - 1 });
	}
	for (; _mPosition.y > TargetPosition.y && remaningMoves > 0; remaningMoves--) {
		_mPosition.y--;
		game->UpdateCharacterPositionInMap(this, Vector2i{ _mPosition.x, _mPosition.y + 1 });
	}
	std::cout << "F moved to: (" << _mPosition.x << "," << _mPosition.y << ")" << std::endl;
}

void Faucheur::Reward(std::vector<std::shared_ptr<Character>> GameCharacters)
{
}

void Faucheur::OnDeath()
{
}

std::vector<Vector2i> Faucheur::GetReachablePosition(Vector2i TargetPosition, std::vector<std::string> GameMap)
{
	std::vector<Vector2i> availablePositions = { {TargetPosition.x - 1, TargetPosition.y }, {TargetPosition.x + 1, TargetPosition.y}, 
											{ TargetPosition.x, TargetPosition.y - 1}, {TargetPosition.x, TargetPosition.y + 1} };
	std::vector<Vector2i> reachablePositions;

	for (int i = 0; i < availablePositions.size(); i++) {
		if (availablePositions[i].x > 0 && availablePositions[i].x < (GameMap[0].size() - 1)
			&& availablePositions[i].y > 0 && availablePositions[i].y < (GameMap.size() - 1)
			&& GameMap[availablePositions[i].y][availablePositions[i].x] == ' ')
			reachablePositions.push_back(availablePositions[i]);
	}
	return reachablePositions;
}

std::vector<Vector2i> Faucheur::GetPathTo(Vector2i TargetPosition)
{
	int length = 0;
	int x = _mPosition.x, y = _mPosition.y;
	std::vector<Vector2i> pathway;
	for (; x < TargetPosition.x; ) {
		x++;
		pathway.push_back({ x, y });
	}
	for (; x > TargetPosition.x; ) {
		x--;
		pathway.push_back({ x, y });
	}

	for (; y < TargetPosition.y; ) {
		y++;
		pathway.push_back({ x, y });
	}
	for (; y > TargetPosition.y; ) {
		y--;
		pathway.push_back({ x, y });
	}
	return pathway;
}

void Faucheur::FindNextMove(std::shared_ptr<Character> TargetPlayer, std::vector<std::string> GameMap)
{
	std::vector<Vector2i> reachablePositions = GetReachablePosition(TargetPlayer->GetPosition(), GameMap);
	std::vector<std::vector<Vector2i>> pathWays;
	Game* game = static_cast<Game*>(_mGame);

	if (reachablePositions.size() <= 0)
		return;
	if (game == nullptr)
		exit(84);
	int bestSize = -1;
	int bestPath = -1;

	for (int i = 0; i < reachablePositions.size(); i++) {
		auto tmp = GetPathTo(reachablePositions[i]);
		if (tmp.size() < bestSize) {
			bestSize = tmp.size();
			bestPath = i;
		}
		pathWays.push_back(tmp);
	}
	if (bestPath != -1) {
		for (int i = 0, remaningMoves = _mMoveLimit; i < pathWays[bestPath].size() && remaningMoves > 0; i++, remaningMoves--) {
			Vector2i prevPos = GetPosition();
			SetPosition(pathWays[bestPath][i]);
			game->UpdateCharacterPositionInMap(this, prevPos);
		}
		std::cout << "F moved to: (" << _mPosition.x << "," << _mPosition.y << ")" << std::endl;
	}
}

void Faucheur::Attack(std::shared_ptr<Character> TargetPlayer)
{
	Vector2i TargetPosition = TargetPlayer->GetPosition();
	std::vector<Vector2i> availablePositions = { {TargetPosition.x - 1, TargetPosition.y }, {TargetPosition.x + 1, TargetPosition.y},
												{ TargetPosition.x, TargetPosition.y - 1}, {TargetPosition.x, TargetPosition.y + 1} };

	for (int i = 0; i < availablePositions.size(); i++) {
		if (_mPosition.y == availablePositions[i].y && _mPosition.x == availablePositions[i].y) {
			TargetPlayer->TakeDamage(_mDamageAmount);
			std::cout << "F attacked the Player" << std::endl;
			_getch();
		}
	}
}
