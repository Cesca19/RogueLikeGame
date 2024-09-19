#include "Golem.h"
#include "Game.h"

Golem::Golem(int Hp, int DamageAmount, char Skin, int DamageIncreaseAmount) : 
	Monster(Hp, DamageAmount, Skin), _mResistanceWeight(50), _mDamageIncreaseAmount(DamageIncreaseAmount)
{
}

Golem::~Golem()
{
}

void Golem::Update(std::vector<std::shared_ptr<Character>> GameCharacters, std::vector<std::string> GameMap)
{
	//golem attck from f
	std::shared_ptr<Player> player = nullptr;
	Game* game = static_cast<Game*>(_mGame);
	
	for (int i = 0; i < GameCharacters.size(); i++) {
		player = std::dynamic_pointer_cast<Player>(GameCharacters[i]);
		if (player != nullptr)
			break;
	}
	if (IsPlayerClose(player)) {
		player->TakeDamage(_mDamageAmount, this);
		game->AddToActionLog( GetColor() + "Golem (G)"+  RESET + " attacked you and you loose " + std::to_string(_mDamageAmount) + " HP");
	}
}

void Golem::MoveTo(Vector2i TargetPosition)
{
}

void Golem::Reward(std::vector<std::shared_ptr<Character>> GameCharacters)
{
	for (int i = 0; i < GameCharacters.size(); i++) {
		std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(GameCharacters[i]);
		if (player != nullptr)
			player->InreaseDamage(_mDamageIncreaseAmount);
	}
}

void Golem::TakeDamage(int Amount, Character * Source)
{
	Game* game = static_cast<Game*>(_mGame);
	if (game == nullptr)
		exit(84);

	if (dynamic_cast<Player*>(Source)) {
		bool canResit = ((rand() % 101) <= _mResistanceWeight) ? true : false;
		if (canResit) {
			_mResistanceWeight -= 10;
			game->AddToActionLog(GetColor() + "Golem (G)" + RESET + " resist to your attack");
		}
		else {
			_mResistanceWeight += 10;
			_mHp -= Amount;
			if (_mHp <= 0)
				OnDeath();
		}
		if (_mResistanceWeight < 10 || _mResistanceWeight >= 90)
			_mResistanceWeight = 40;
	}
	else {
		_mHp -= Amount;
		if (_mHp <= 0)
			OnDeath();
	}
}

void Golem::OnDeath()
{
	Game* game = static_cast<Game*>(_mGame);
	if (game == nullptr)
		exit(84);
	std::shared_ptr<Player> player = game->GetPlayer();
	player->InreaseDamage(_mDamageIncreaseAmount);
	game->AddToActionLog(GetColor() + "Golem (G)" + RESET + " increased your ATK by " + std::to_string(_mDamageIncreaseAmount));
}
