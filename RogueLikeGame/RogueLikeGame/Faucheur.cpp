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
		if (startPos.x != _mPosition.x || startPos.y != _mPosition.y)
			game->AddToActionLog(GetColor() + "Faucheur (F)" + RESET + " moved from (" + std::to_string(startPos.x) + "," + std::to_string(startPos.y) + ") to ("
				+ std::to_string(_mPosition.x) + "," + std::to_string(_mPosition.y) + ")");
		Attack(player);
	}
}

Vector2i Faucheur::GetLeastF(std::unordered_map<Vector2i, std::pair<Vector2i, Vector2i>> map)
{
	int minF = -1, currentF = -1;
	Vector2i leastF = {-1, -1};
	for (const auto& point : map) {
		currentF = point.second.first.x + point.second.first.y;
		if (currentF < minF && currentF >= 0 || minF == -1) {
			minF = currentF;
			leastF = point.first;
		}
	}
	return leastF;
}

std::vector<Vector2i> Faucheur::GetSuccessors(Vector2i parent, std::vector<std::string> GameMap)
{
	std::vector<Vector2i> availablePositions = { {parent.x - 1, parent.y }, {parent.x + 1, parent.y},
												{ parent.x, parent.y - 1}, {parent.x, parent.y + 1} };
	std::vector<Vector2i> position;

	for (int i = 0; i < availablePositions.size(); i++) {
		if (availablePositions[i].y < 0 || availablePositions[i].y >= GameMap.size())
			continue;
		if (availablePositions[i].x < 0 || availablePositions[i].x >= GameMap[0].size())
			continue;
		if (GameMap[availablePositions[i].y][availablePositions[i].x] == ' ' || GameMap[availablePositions[i].y][availablePositions[i].x] == '.' )
			position.push_back(availablePositions[i]);
	}
	return position;
}

std::vector<Vector2i> Faucheur::GetPathway(Vector2i Dest, std::unordered_map<Vector2i, std::pair<Vector2i, Vector2i>> PrevPos)
{
	std::vector<Vector2i> path;

	path.push_back(Dest);
	Vector2i last = Dest;

	while (1) {
		auto tmp = PrevPos.find(last);
		Vector2i prev = tmp->second.second;
		if (prev.x == -1 && prev.y == -1)
			break;
		path.push_back(prev); 
		last = prev;
	}
	return path;
}



std::vector<Vector2i> Faucheur::MoveTo(Vector2i TargetPosition, std::vector<std::string> GameMap)
{
	std::vector<Vector2i> path;
	///                  pos                g h     parentPos
	std::unordered_map<Vector2i, std::pair<Vector2i, Vector2i>> openList;
	std::unordered_map<Vector2i, std::pair<Vector2i, Vector2i>> closedList;
	openList[_mPosition] = std::make_pair(Vector2i{ 0, 0 }, Vector2i{ -1, -1 });

	while (!openList.empty()) {
		const Vector2i leastF = GetLeastF(openList);
		std::pair<Vector2i, Vector2i> leastFValue = openList[leastF];

		openList.erase(openList.find(leastF));
		std::vector<Vector2i> successors = GetSuccessors(leastF, GameMap);
		for (int i = 0; i < successors.size(); i++) {
			if (successors[i].x == TargetPosition.x && successors[i].y == TargetPosition.y) {
				// goal reached
				closedList[leastF] = leastFValue;
				closedList[TargetPosition] = std::make_pair(Vector2i{ 0, 0 }, leastF);
				return GetPathway(TargetPosition , closedList);
				//break;
			}
			else {
				int g = leastFValue.first.x + 1;
				int h = std::abs(successors[i].x - TargetPosition.x) + std::abs(successors[i].y - TargetPosition.y);
				auto ans = openList.find(successors[i]);
				if (ans != openList.end())
					continue;
				if (ans != openList.end() && (ans->first.x + ans->first.y) < (g + h))
					continue;
				auto ansClosed = closedList.find(successors[i]);
				if (ansClosed != closedList.end())
					continue;
				if (ansClosed != closedList.end() && (ansClosed->first.x + ansClosed->first.y) < (g + h))
					continue;
				openList[successors[i]] = std::make_pair(Vector2i{ g, h}, leastF);
			}
		}
		closedList[leastF] = leastFValue;
	}
	return path;
}

void Faucheur::MoveTo(Vector2i TargetPosition)
{
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

std::vector<Vector2i> Faucheur::GetPathTo(Vector2i TargetPosition, std::vector<std::string> GameMap)
{
	std::vector<Vector2i> pathway = MoveTo(TargetPosition, GameMap);
	std::vector<Vector2i> ans;
	for (int i = pathway.size() - 1; i >= 0; i--)
		ans.push_back(pathway[i]);
	return ans;
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