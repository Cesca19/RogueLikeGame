#include "Monster.h"

Monster::Monster(int Hp, int DamageAmount, char Skin) : Character(Hp, DamageAmount, Skin), _mMoveLimit(5)
{
}

Monster::~Monster()
{
}

bool Monster::IsPlayerClose(std::shared_ptr<Character> TargetPlayer)
{
	Vector2i TargetPosition = TargetPlayer->GetPosition();
	std::vector<Vector2i> availablePositions = { {TargetPosition.x - 1, TargetPosition.y }, {TargetPosition.x + 1, TargetPosition.y},
												{ TargetPosition.x, TargetPosition.y - 1}, {TargetPosition.x, TargetPosition.y + 1} };

	for (int i = 0; i < availablePositions.size(); i++)
		if (_mPosition.y == availablePositions[i].y && _mPosition.x == availablePositions[i].x)
			return true;
	return false;
}

void Monster::MoveTo(Vector2i TargetPosition)
{
}

Vector2i Monster::GetLeastF(std::unordered_map<Vector2i, std::pair<Vector2i, Vector2i>> map)
{
	int minF = -1, currentF = -1;
	Vector2i leastF = { -1, -1 };
	for (const auto& point : map) {
		currentF = point.second.first.x + point.second.first.y;
		if (currentF < minF && currentF >= 0 || minF == -1) {
			minF = currentF;
			leastF = point.first;
		}
	}
	return leastF;
}

std::vector<Vector2i> Monster::GetSuccessors(Vector2i parent, std::vector<std::string> GameMap)
{
	std::vector<Vector2i> availablePositions = { {parent.x - 1, parent.y }, {parent.x + 1, parent.y},
												{ parent.x, parent.y - 1}, {parent.x, parent.y + 1} };
	std::vector<Vector2i> position;

	for (int i = 0; i < availablePositions.size(); i++) {
		if (availablePositions[i].y < 0 || availablePositions[i].y >= GameMap.size())
			continue;
		if (availablePositions[i].x < 0 || availablePositions[i].x >= GameMap[0].size())
			continue;
		if (GameMap[availablePositions[i].y][availablePositions[i].x] == ' ' || GameMap[availablePositions[i].y][availablePositions[i].x] == '.')
			position.push_back(availablePositions[i]);
	}
	return position;
}

std::vector<Vector2i> Monster::GetPathway(Vector2i Dest, std::unordered_map<Vector2i, std::pair<Vector2i, Vector2i>> PrevPos)
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



std::vector<Vector2i> Monster::MoveTo(Vector2i TargetPosition, std::vector<std::string> GameMap)
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
				return GetPathway(TargetPosition, closedList);
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
				openList[successors[i]] = std::make_pair(Vector2i{ g, h }, leastF);
			}
		}
		closedList[leastF] = leastFValue;
	}
	return path;
}

std::vector<Vector2i> Monster::GetPathTo(Vector2i TargetPosition, std::vector<std::string> GameMap)
{
	std::vector<Vector2i> pathway = MoveTo(TargetPosition, GameMap);
	std::vector<Vector2i> ans;
	for (int i = pathway.size() - 1; i >= 0; i--)
		ans.push_back(pathway[i]);
	return ans;
}