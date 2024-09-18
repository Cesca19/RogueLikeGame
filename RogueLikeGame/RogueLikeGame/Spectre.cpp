#include "Spectre.h"
#include "Game.h"
Spectre::Spectre(int Hp, int DamageAmount, char Skin) : Monster(Hp, DamageAmount, Skin)
{
}

Spectre::~Spectre()
{
}

void Spectre::Update(std::vector<std::shared_ptr<Character>> GameCharacters, std::vector<std::string> GameMap)
{
	std::shared_ptr<Player> player = nullptr;
	Vector2i targetPosition;
	bool canFlee = false;
	for (int i = 0; i < GameCharacters.size(); i++) {
		player = std::dynamic_pointer_cast<Player>(GameCharacters[i]);
		if (player != nullptr)
			break;
	}
	if (player != nullptr) {
		targetPosition = FleeFrom(player, canFlee, GameMap);
		if (canFlee) {
			MoveTo(targetPosition);
			std::cout << "Spectre moved to: (" << targetPosition.x << "," << targetPosition.y << ")" << std::endl;
			_getch();
		}
	}

}

void Spectre::MoveTo(Vector2i TargetPosition)
{
	Game* game = static_cast<Game*>(_mGame);
	if (game == nullptr)
		exit(84);

	//replace with bfs or a*
	for (; _mPosition.x < TargetPosition.x; ) {
		_mPosition.x ++;
		game->UpdateCharacterPositionInMap(this, Vector2i{ _mPosition.x - 1, _mPosition.y });
	}
	for (; _mPosition.x > TargetPosition.x; ) {
		_mPosition.x--;
		game->UpdateCharacterPositionInMap(this, Vector2i{ _mPosition.x + 1, _mPosition.y });
	}

	for (; _mPosition.y < TargetPosition.y; ) {
		_mPosition.y++;
		game->UpdateCharacterPositionInMap(this, Vector2i{ _mPosition.x, _mPosition.y - 1});
	}
	for (; _mPosition.y > TargetPosition.y; ) {
		_mPosition.y--;
		game->UpdateCharacterPositionInMap(this, Vector2i{ _mPosition.x, _mPosition.y + 1});
	}
}

void Spectre::Reward(std::vector<std::shared_ptr<Character>> GameCharacters)
{
}

Vector2i Spectre::FleeFrom(std::shared_ptr<Character> Target, bool &CanFlee, std::vector<std::string> GameMap)
{ 
	Vector2i targetPos = Target->GetPosition();
	Vector2i fleeVector = { (_mPosition.x - targetPos.x), (_mPosition.y - targetPos.y) }; // add a variable offset
	Vector2i  destination = { _mPosition.x + fleeVector.x,  _mPosition.y - fleeVector.y };
	std::cout << "p: " << targetPos.x << ":" << targetPos.y << std::endl;
	std::cout << "g: " <<  _mPosition.x << ":" << _mPosition.y << std::endl;
	std::cout << "offset: " << fleeVector.x << ":" << fleeVector.y << std::endl;
	std::cout << destination.x << ":" << destination.y << std::endl;
	
	
	if (destination.y > 0 && destination.y < (GameMap.size() - 1 ) && destination.x > 0 && destination.x < (GameMap[0].size() - 1))
		CanFlee = (GameMap[destination.y][destination.x] == ' ') ? true : false;
	return destination;
}

void Spectre::OnDeath()
{
}
