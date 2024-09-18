#include "Game.h"
#include <fstream>
#include <iostream>
#include <conio.h>
#include <iomanip>
#include <sstream>
#include <deque>

#include "Faucheur.h"
#include "Golem.h"
#include "Navigator.h"
#include "Spectre.h"

Game::Game() : _mTurn(0) {}

Game::~Game() {}

void Game::Init() {
    LoadMap();
    for (int y = 0; y < _mMap.size(); ++y) {
        for (int x = 0; x < _mMap[y].length(); ++x) {
            switch (_mMap[y][x]) {
            case '@': {
                _mPlayer = std::make_shared<Player>(100, 20, '@');
                _mPlayer->SetPosition(Vector2i{ x, y });
                _mPlayer->SetGame(this);
                _mCharacters.push_back(_mPlayer);
                break;
            }
               
            case 'G': {
                auto golem = std::make_shared<Golem>(50, 20, 'G', 10);
                golem->SetPosition(Vector2i{ x, y });
                golem->SetGame(this);
                _mMonsters.push_back(golem);
                _mGameMonsters.push_back(golem);
                _mCharacters.push_back(_mMonsters.back());
                break;
            }
                
            case 'S': {
                auto spectre = std::make_shared<Spectre>(50, 10, 'S');
                spectre->SetPosition(Vector2i{ x, y });
                spectre->SetGame(this);
                _mGameMonsters.push_back(spectre);
                _mMonsters.push_back(spectre);
                _mCharacters.push_back(_mMonsters.back());
                break;
            }
                
            case 'F': {
                auto faucheur = std::make_shared<Faucheur>(50, 30, 'F');
                faucheur->SetPosition(Vector2i{ x, y });
                faucheur->SetGame(this);
                _mMonsters.push_back(faucheur);
                _mGameMonsters.push_back(faucheur);
                _mCharacters.push_back(_mMonsters.back());
                break;
            }
            case '#': {
                _mNavigator = std::make_shared<Navigator>(x, y, '#');
                break;
            }

            }
        }
    }

    for (size_t y = 0; y < _mMap.size(); ++y) {
        for (size_t x = 0; x < _mMap[y].length(); ++x) {
            if (_mMap[y][x] == '@' || _mMap[y][x] == '#') {
                _mMap[y][x] = ' ';
            }
        }
    }
    _mTurn = 0;
}

void Game::LoadMap() {
    std::ifstream mapFile("map/map_1.txt");
    if (!mapFile.is_open()) {
        std::cerr << "Failed to open map file!" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(mapFile, line)) {
        _mMap.push_back(line);
    }

    mapFile.close();
}

void Game::Run() {
    Render();
	while (true) {
        switch (_mTurn) {
        case 0:
            HandleInput();
            _mTurn = 1;
            break;
        case 1:
            for (int i = 0; i < _mMonsters.size(); i++)
                _mMonsters[i]->Update(_mCharacters, _mMap);
            _mTurn = 0;
            break;
        default:
            break;
        }
	}
}


void Game::Render() {
    system("cls");

    std::vector<std::string> renderMap = _mMap;

    if (_mNavigator && _mCurrentState == GameState::Moving) {
        Vector2i position = _mNavigator->GetPosition();
        if (IsValidMove(position)) {
            renderMap[position.y][position.x] = _mNavigator->GetSymbol();
        }
    }

    for (const auto& character : _mCharacters) {
        Vector2i position = character->GetPosition();
        renderMap[position.y][position.x] = character->GetSymbol();
    }

    for (size_t i = 0; i < _mAttackableMonsters.size(); ++i) {
        const auto& monster = _mAttackableMonsters[i];
        Vector2i position = monster->GetPosition();
        renderMap[position.y][position.x] = (i == _mSelectedMonsterIndex && _mCurrentState == GameState::Attacking) ? '!' : _mAttackSymbol;
    }

	std::vector<std::string> statsOutput = RenderStats();
    size_t maxHeight = std::max(renderMap.size(), statsOutput.size());
    for (size_t i = 0; i < maxHeight; ++i) {
        if (i < renderMap.size()) {
            std::cout << renderMap[i];
        }
        else {
            std::cout << std::string(renderMap[0].length(), ' ');
        }
        std::cout << "    ";

        if (i < statsOutput.size()) {
            std::cout << statsOutput[i];
        }
        std::cout << std::endl;
    }

    if (_mNavigator && _mCurrentState == GameState::Moving) {
        std::cout << "\nNavigator position: (" << _mNavigator->GetX() << ", " << _mNavigator->GetY() << ")" << std::endl;
    }

    if (_mCurrentState == GameState::Attacking) {
        std::cout << "\nAttackable monsters:" << std::endl;
        for (size_t i = 0; i < _mAttackableMonsters.size(); ++i) {
            const auto& monster = _mAttackableMonsters[i];
            std::cout << (i == _mSelectedMonsterIndex ? "-> " : "   ")
                << monster->GetSymbol() << " at (" << monster->GetPosition().x << ", " << monster->GetPosition().y << ")" << std::endl;
        }
    }

    std::cout << "\nControls:\n";
    if (_mCurrentState == GameState::Moving) {
        std::cout << "Arrow keys: Move navigator\n";
        std::cout << "Space: Move player to navigator\n";
        std::cout << "Enter: Enter attack mode\n";
        std::cout << "Esc: Quit game\n";
    }
    else if (_mCurrentState == GameState::Attacking) {
        std::cout << "Arrow keys: Select monster\n";
        std::cout << "Space: Confirm attack\n";
        std::cout << "Esc: Exit attack mode\n";
    }
}

std::vector<std::string> Game::RenderStats() {
    std::vector<std::string> statsOutput;
    std::stringstream ss;

    ss << "Player Stats:\n";
    ss << std::setw(15) << std::left << "Name:" << _mPlayer->GetSymbol() << "\n";
    ss << std::setw(15) << std::left << "HP:" << _mPlayer->GetHp() << "\n";
    ss << std::setw(15) << std::left << "ATK:" << _mPlayer->GetDamageAmount() << "\n\n";

    if (_mCurrentState == GameState::Attacking && _mSelectedMonsterIndex < _mAttackableMonsters.size()) {
        const auto& monster = _mAttackableMonsters[_mSelectedMonsterIndex];
        ss << "Selected Monster Stats:\n";
        ss << std::setw(15) << std::left << "Name:" << monster->GetSymbol() << "\n";
        ss << std::setw(15) << std::left << "HP:" << monster->GetHp() << "\n";
        ss << std::setw(15) << std::left << "ATK:" << monster->GetDamageAmount() << "\n\n";
    }

    ss << "Recent Actions:\n";
    for (const auto& action : _mActionLog) {
        ss << action << "\n";
    }

    std::string line;
    while (std::getline(ss, line)) {
        statsOutput.push_back(line);
    }

    return statsOutput;
}


void Game::HandleInput() {
    int key = _getch();
    bool actionTaken = false;

    while (!actionTaken) {
        if (_mCurrentState == GameState::Moving) {
            if (key == 224) { // Arrow key pressed
                key = _getch();
                int dx = 0, dy = 0;
                switch (key) {
                case 72: dy = -1; break; // Up arrow
                case 80: dy = 1; break;  // Down arrow
                case 75: dx = -1; break; // Left arrow
                case 77: dx = 1; break;  // Right arrow
                }
                MoveNavigator(dx, dy);
            }
            else if (key == 13) { // Enter key
                if (!GetAttackableMonsters().empty()) {
                    EnterAttackMode();
                    actionTaken = true;
                }
            }
            else if (key == 32) { // Space key
                if (_mNavigator) {
                    Move();
                    actionTaken = true;
                }
            }
        }

        else if (_mCurrentState == GameState::Attacking) {
            if (key == 224) {
                key = _getch();
                switch (key) {
                case 72: // Up arrow
                case 75: // Left arrow
                    SelectNextMonster(-1);
                    break;
                case 80: // Down arrow
                case 77: // Right arrow
                    SelectNextMonster(1);
                    break;
                }
            }
            else if (key == 32) { // Enter key
                PerformAttack();
                actionTaken = true;
            }
            else if (key == 27) { // Esc key
                ExitAttackMode();
            }
        }

        Render();
        if (!actionTaken) {
            key = _getch();
        }
    }
}

void Game::Move() {
    if (_mNavigator && IsValidMove(_mNavigator->GetPosition())) {
        Vector2i oldPos = _mPlayer->GetPosition();
        _mPlayer->SetPosition(_mNavigator->GetPosition());
        AddToActionLog("Player moved from (" + std::to_string(oldPos.x) + "," +
            std::to_string(oldPos.y) + ") to (" +
            std::to_string(_mPlayer->GetPosition().x) + "," +
            std::to_string(_mPlayer->GetPosition().y) + ")");
    }
}

bool Game::IsValidMove(Vector2i position) {
    if (_mMap[position.y][position.x] == ' ') {
        return true;
    }
    return false;
}

void Game::MoveNavigator(int dx, int dy) {
    if (_mNavigator) {
	    const int newX = _mNavigator->GetX() + dx;
	    const int newY = _mNavigator->GetY() + dy;
        if (IsValidMove(Vector2i { newX, newY })) {
            _mNavigator->Move(dx, dy);
        }
    }
}

std::vector<std::shared_ptr<Monster>> Game::GetAttackableMonsters() {
    std::vector<std::shared_ptr<Monster>> attackableMonsters;
    Vector2i playerPos = _mPlayer->GetPosition();

    for (const auto& monster : _mMonsters) {
        Vector2i monsterPos = monster->GetPosition();
        int dx = abs(playerPos.x - monsterPos.x);
        int dy = abs(playerPos.y - monsterPos.y);

        if ((dx == 1 && dy == 0) || (dx == 0 && dy == 1)) {
            attackableMonsters.push_back(monster);
        }
    }

    return attackableMonsters;
}

std::vector<std::string> Game::UpdateCharacterPositionInMap(Character *Target, Vector2i PrevPosition)
{
    Vector2i position = Target->GetPosition();
    _mMap[position.y][position.x] = Target->GetSymbol();
    _mMap[PrevPosition.y][PrevPosition.x] = ' ';
    Render();
    return _mMap;
}

void Game::EnterAttackMode() {
    _mCurrentState = GameState::Attacking;
    _mAttackableMonsters = GetAttackableMonsters();
    _mSelectedMonsterIndex = 0;
}

void Game::ExitAttackMode() {
    _mCurrentState = GameState::Moving;
    _mAttackableMonsters.clear();
    _mSelectedMonsterIndex = 0;
}

void Game::SelectNextMonster(int direction) {
    if (_mAttackableMonsters.empty()) return;
    _mSelectedMonsterIndex = (_mSelectedMonsterIndex + direction + _mAttackableMonsters.size()) % _mAttackableMonsters.size();
}

void Game::AttackMonster(std::vector<std::shared_ptr<Monster>>::const_reference monster) {
    int damage = _mPlayer->GetDamageAmount();
    monster->TakeDamage(damage, _mPlayer.get());

    AddToActionLog("Player attacked " + std::string(1, monster->GetSymbol()) + "who has now " + std::to_string(monster->GetHp()));
        //" for " + std::to_string(damage) + " damage");

    if (monster->GetHp() <= 0) {
        _mMonsters.erase(std::remove(_mMonsters.begin(), _mMonsters.end(), monster), _mMonsters.end());
        _mCharacters.erase(std::remove(_mCharacters.begin(), _mCharacters.end(), monster), _mCharacters.end());

        Vector2i position = monster->GetPosition();
        _mMap[position.y][position.x] = ' ';

        AddToActionLog(std::string(1, monster->GetSymbol()) + " is defeated");
    }
}

void Game::PerformAttack() {
    if (_mSelectedMonsterIndex < _mAttackableMonsters.size()) {
        AttackMonster(_mAttackableMonsters[_mSelectedMonsterIndex]);
    }
    ExitAttackMode();
}

void Game::AddToActionLog(const std::string& action) {
    _mActionLog.push_front(action);
    if (_mActionLog.size() > MAX_LOG_ENTRIES) {
        _mActionLog.pop_back();
    }
    Render();
}

std::vector<std::shared_ptr<Character>> Game::GetAllMonsters()
{
    return _mGameMonsters;
}

std::shared_ptr<Player> Game::GetPlayer()
{
    return _mPlayer;
}
