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
	Game* game = static_cast<Game*>(_mGame);
	Vector2i startPos = _mPosition;
	Vector2i targetPosition = {};
	for (int i = 0; i < GameCharacters.size(); i++) {
		player = std::dynamic_pointer_cast<Player>(GameCharacters[i]);
		if (player != nullptr)
			break;
	}
	if (player != nullptr) {
		GameMap[player->GetY()][player->GetX()] = '@';
		if (!IsPlayerClose(player))
			FindNextMove(player, GameMap);
		else
			Attack(player);
		if (startPos != _mPosition)
			game->AddToActionLog(GetColor() + "Faucheur (F)" + RESET + " moved from (" + std::to_string(startPos.x) + "," + std::to_string(startPos.y) + ") to ("
				+ std::to_string(_mPosition.x) + "," + std::to_string(_mPosition.y) + ")");
	}
}

void Faucheur::Reward(std::vector<std::shared_ptr<Character>> GameCharacters)
{
	Game* game = static_cast<Game*>(_mGame);
	for (int i = 0; i < GameCharacters.size(); i++) {
		if (GameCharacters[i].get() != this) {
			GameCharacters[i]->TakeDamage(_mDamageAmount, this);
			game->AddToActionLog(GetColor() + "Faucheur (F)" + RESET + " attacked " + std::string{ GameCharacters[i]->GetSymbol() } +
				" at (" + std::to_string(GameCharacters[i]->GetX()) + "," + std::to_string(GameCharacters[i]->GetY()) + ") who has now " + 
				std::to_string(GameCharacters[i]->GetHp()) + " HP");
		}
	}
}

void Faucheur::OnDeath()
{
	Game* game = static_cast<Game*>(_mGame);
	if (game == nullptr)
		exit(84);
	Reward(game->GetAllMonsters());
}

std::vector<Vector2i> Faucheur::GetReachablePosition(Vector2i TargetPosition, std::vector<std::string> GameMap)
{
	std::vector<Vector2i> availablePositions = { {TargetPosition.x - 1, TargetPosition.y }, {TargetPosition.x + 1, TargetPosition.y}, 
											{ TargetPosition.x, TargetPosition.y - 1}, {TargetPosition.x, TargetPosition.y + 1} };
	std::vector<Vector2i> reachablePositions;

	for (int i = 0; i < availablePositions.size(); i++) {
		if (availablePositions[i].x > 0 && availablePositions[i].x < (GameMap[0].size() - 1)
			&& availablePositions[i].y > 0 && availablePositions[i].y < (GameMap.size() - 1)
			&& (GameMap[availablePositions[i].y][availablePositions[i].x] == ' ' || GameMap[availablePositions[i].y][availablePositions[i].x] == '.'))
			reachablePositions.push_back(availablePositions[i]);
	}
	return reachablePositions;
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
		auto tmp = GetPathTo(reachablePositions[i], GameMap);
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
	}

}

void Faucheur::Attack(std::shared_ptr<Character> TargetPlayer)
{
	Vector2i TargetPosition = TargetPlayer->GetPosition();
	std::vector<Vector2i> availablePositions = { {TargetPosition.x - 1, TargetPosition.y }, {TargetPosition.x + 1, TargetPosition.y},
												{ TargetPosition.x, TargetPosition.y - 1}, {TargetPosition.x, TargetPosition.y + 1} };
	Game* game = static_cast<Game*>(_mGame);
	if (game == nullptr)
		exit(84);


	for (int i = 0; i < availablePositions.size(); i++) {
		if (_mPosition.y == availablePositions[i].y && _mPosition.x == availablePositions[i].x) {
			TargetPlayer->TakeDamage(_mDamageAmount, this);
			game->AddToActionLog(GetColor() + "Faucheur (F)" + RESET + " attacked you and you loose " + std::to_string(_mDamageAmount) + " HP");
		}
	}
}