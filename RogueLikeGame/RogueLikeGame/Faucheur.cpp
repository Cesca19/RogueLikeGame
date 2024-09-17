#include "Faucheur.h"

Faucheur::Faucheur(int Hp, int DamageAmount, char Skin) : Monster(Hp, DamageAmount, Skin)
{
}

Faucheur::~Faucheur()
{
}

void Faucheur::Update(std::vector<std::shared_ptr<Character>> GameCharacters, std::vector<std::string> GameMap)
{
}

void Faucheur::MoveTo(Vector2i TargetPosition)
{
}

void Faucheur::Reward(std::vector<std::shared_ptr<Character>> GameCharacters)
{
}

void Faucheur::OnDeath()
{
}
