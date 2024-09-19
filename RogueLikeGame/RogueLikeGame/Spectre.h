#pragma once

#include "Monster.h"
#include "Player.h"

struct Pair_hash {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        // Combiner les deux hash codes
        return h1 ^ (h2 << 1);
    }
};

class Spectre : public Monster
{
public:
	Spectre(int Hp, int DamageAmount, char Skin);
	~Spectre();
	void Update(std::vector<std::shared_ptr<Character>> GameCharacters, std::vector<std::string> GameMap) override;
	void Reward(std::vector<std::shared_ptr<Character>> GameCharacters) override;
	void OnDeath() override;
private:
    Vector2i GetRunawayDirection(std::vector<std::string> GameMap, Vector2i Dest);
    void Attack(std::shared_ptr<Character> TargetPlayer);
};

