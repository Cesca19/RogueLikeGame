#include "Player.h"

Player::Player(int Hp, int DamageAmount, char Skin) : Character(Hp, DamageAmount, Skin), _mMaxHp(Hp)
{
}

Player::~Player()
{
}

void Player::Update(std::vector<std::shared_ptr<Character>> GameCharacters, std::vector<std::string> GameMap)
{
}

void Player::MoveTo(Vector2i TargetPosition)
{
	_mPosition = TargetPosition;
}

void Player::InreaseDamage(int Amount)
{
	_mDamageAmount += Amount;
}

void Player::IncreaseHp(int Hp) {
	_mHp += Hp;
	if (_mHp > _mMaxHp) _mHp = _mMaxHp;
}

void Player::RestoreHp()
{
	_mHp = _mMaxHp;
}

void Player::OnDeath()
{
}

