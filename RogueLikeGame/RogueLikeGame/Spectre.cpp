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
			std::cout << "fleeeeeeeeeeeeeeeeeeeeee" << std::endl;
			//MoveTo(targetPosition);
		}
	}

}

void Spectre::MoveTo(Vector2i TargetPosition)
{
	int i = 1, j = 1;
	if (_mPosition.x > TargetPosition.x)
		i = -1;
	if (_mPosition.y > TargetPosition.y)
		j = -1;
	Game* game = static_cast<Game*>(_mGame);
	if (game == nullptr) {
		std::cout << "errrrrrrrrrrrrrrrrrrrrrrorr" << std::endl;
		exit(84);
	}

	for (; _mPosition.x != TargetPosition.x; ) {
		_mPosition.x += i;
		game->UpdateCharacterPositionInMap(this, Vector2i{ _mPosition.x - i, _mPosition.y });
	}
	for (; _mPosition.y != TargetPosition.y; _mPosition.y += j) {
		_mPosition.y += j;
		game->UpdateCharacterPositionInMap(this, Vector2i{ _mPosition.x, _mPosition.y - j });
	}
}

void Spectre::Reward(std::vector<std::shared_ptr<Character>> GameCharacters)
{
}

Vector2i Spectre::FleeFrom(std::shared_ptr<Character> Target, bool &CanFlee, std::vector<std::string> GameMap)
{ 
	Vector2i targetPos = Target->GetPosition();
	Vector2i fleeVector = { - (_mPosition.x - targetPos.x),  _mPosition.y - targetPos.y };
	Vector2i  destination = { _mPosition.x + fleeVector.x,  _mPosition.y - fleeVector.y };
	
	CanFlee = (GameMap[destination.y][destination.x] == ' ') ? true : false;
	return destination;
}

void Spectre::OnDeath()
{
}
