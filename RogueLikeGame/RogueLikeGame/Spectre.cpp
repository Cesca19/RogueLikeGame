#include "Spectre.h"
#include "Game.h"
Spectre::Spectre(int Hp, int DamageAmount, char Skin) : Monster(Hp, DamageAmount, Skin)
{
}

Spectre::~Spectre()
{
}

Vector2i Spectre::GetRunawayDirection(std::vector<std::string> GameMap, Vector2i Dest)
{
    std::pair<int, int> ans = { -1, -1};
    std::set<std::pair<int, int>> seen;
    std::deque<std::tuple<int, int, int>> to_see;
    std::unordered_map<std::pair<int, int>, std::pair<int, int>, Pair_hash> parents;
    std::tuple<int, int, int> current;
    int start_x = _mPosition.x, start_y = _mPosition.y, target_x = Dest.x, target_y = Dest.y;
    int steps = 0;
    double highest_distance = sqrt((pow(target_x - start_x, 2)) + (pow(target_y - start_y, 2)));
    //    double highest_distance = -std::numeric_limits<double>::infinity();
    double current_dist = 0;
    int current_x = 0;
    int current_y = 0;
    int neighbor_x = 0;
    int neighbor_y = 0;
    
    std::vector<std::pair<int, int>> directions = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };


    to_see.push_front({ start_x, start_y, 0 });

    while (!to_see.empty()) {
        current = to_see.back();
        to_see.pop_back();
        current_x = std::get<0>(current);
        current_y = std::get<1>(current);
        steps = std::get<2>(current);
        if (steps > _mMoveLimit)
            break;
        for (const auto& dir : directions) {
            neighbor_x = current_x + dir.first;
            neighbor_y = current_y + dir.second;
            //we doge bade cases
            if (neighbor_x < 0 || neighbor_x >= GameMap[0].size())
                continue;
            if (neighbor_y < 0 || neighbor_y >= GameMap.size())
                continue;
            // if we have already seen this tupple
            if (seen.find({ neighbor_x,neighbor_y }) != seen.end())
                continue;
            // if it is not a space we move on
            if (!(GameMap[neighbor_y][neighbor_x] == ' ' || GameMap[neighbor_y][neighbor_x] == '.'))
                continue;
            to_see.push_front({ neighbor_x, neighbor_y, steps + 1 });
            current_dist = sqrt((pow(target_x - neighbor_x, 2)) + (pow(target_y - neighbor_y, 2)));
            if (current_dist > highest_distance) {
                highest_distance = current_dist;
                ans = { neighbor_x, neighbor_y };
            } 
            seen.insert({ neighbor_x,neighbor_y });
        }
    }
    return Vector2i({ ans.first, ans.second});
}

void Spectre::Attack(std::shared_ptr<Character> TargetPlayer)
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
            game->AddToActionLog(GetColor() + "Spectre (S)" + RESET + " attacked you and you loose " + std::to_string(_mDamageAmount) + " HP");
        }
    }
}

void Spectre::Update(std::vector<std::shared_ptr<Character>> GameCharacters, std::vector<std::string> GameMap)
{
	
	std::shared_ptr<Player> player = nullptr;
    Vector2i targetPosition = {-1, -1};
    Vector2i startPos = _mPosition;
    Game* game = static_cast<Game*>(_mGame);

    if (game == nullptr)
        exit(84);
	for (int i = 0; i < GameCharacters.size(); i++) {
		player = std::dynamic_pointer_cast<Player>(GameCharacters[i]);
		if (player != nullptr)
			break;
	}
    for (const auto& character : GameCharacters) {
        Vector2i position = character->GetPosition();
        GameMap[position.y][position.x] = character->GetSymbol();

    }
	if (player != nullptr) {
        targetPosition = GetRunawayDirection(GameMap, player->GetPosition());
        if (targetPosition != Vector2i{-1, -1}) {
            std::vector<Vector2i> pathway = GetPathTo(targetPosition, GameMap);
            for (int i = 0; i < pathway.size(); i++) {
                Vector2i prevPos = GetPosition();
                SetPosition(pathway[i]);
                game->UpdateCharacterPositionInMap(this, prevPos);
            }
            game->AddToActionLog(GetColor() + "Spectre (F)" + RESET + " moved from (" + std::to_string(startPos.x) + "," + std::to_string(startPos.y) + ") to ("
                + std::to_string(targetPosition.x) + "," + std::to_string(targetPosition.y) + ")");
            return;
        }
        if (IsPlayerClose(player))
            Attack(player);
	}
}

void Spectre::Reward(std::vector<std::shared_ptr<Character>> GameCharacters)
{
}

void Spectre::OnDeath()
{
	Game* game = static_cast<Game*>(_mGame);
	if (game == nullptr)
		exit(84);
	std::shared_ptr<Player> player = game->GetPlayer();
	player->RestoreHp();
	game->AddToActionLog(GetColor() + "Spectre (S)" + RESET + " restaured your HP level");
}
 