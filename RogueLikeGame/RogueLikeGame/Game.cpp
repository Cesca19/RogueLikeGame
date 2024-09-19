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

#define RESET           "\033[0m"
#define RED             "\033[31m"
#define GREEN           "\033[32m"
#define YELLOW          "\033[33m"
#define BLUE            "\033[34m"
#define MAGENTA         "\033[35m"
#define CYAN            "\033[36m"
#define WHITE           "\033[37m"
#define BRIGHT_RED      "\033[91m"
#define BRIGHT_GREEN    "\033[92m"
#define BRIGHT_YELLOW   "\033[93m"
#define BRIGHT_BLUE     "\033[94m"
#define BRIGHT_MAGENTA  "\033[95m"
#define BRIGHT_CYAN     "\033[96m"
#define ORANGE          "\033[38;5;208m"
#define PINK            "\033[38;5;213m"
#define LIME            "\033[38;5;10m"
#define TEAL            "\033[38;5;14m"
#define LAVENDER        "\033[38;5;183m"
#define BROWN           "\033[38;5;130m"
#define GRAY            "\033[38;5;240m"

Game::Game() : _mTurn(0), _mCurrentRoom(1), _mPlayerWon(false) {
    _mColors = {
        RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN,
        BRIGHT_RED, BRIGHT_GREEN, BRIGHT_YELLOW, BRIGHT_BLUE, BRIGHT_MAGENTA, BRIGHT_CYAN,
        ORANGE, PINK, LIME, TEAL, LAVENDER, BROWN, GRAY, WHITE
    };
    _mCurrentColorIndex = 0;
}

Game::~Game() {}

void Game::Init() {
    LoadMap();
    for (int y = 0; y < _mMap.size(); ++y) {
        for (int x = 0; x < _mMap[y].length(); ++x) {
            switch (_mMap[y][x]) {
            case '@': {
                _mPlayer = std::make_shared<Player>(500, 100, '@');
                _mPlayer->SetPosition(Vector2i{ x, y });
                _mPlayer->SetGame(this);
                _mPlayer->SetMoveLength(_mMoveLength);
                _mCharacters.push_back(_mPlayer);
                _mMap[y][x] = ' ';
                CalculateValidMoves(_mPlayer->GetMoveLength());
                break;
            }
               
            case 'G': {
                auto golem = std::make_shared<Golem>(50, 20, 'G', 10, 20);
                golem->SetPosition(Vector2i{ x, y });
                golem->SetGame(this);
                golem->SetColor(GetNextColor());
                _mMonsters.push_back(golem);
                _mGameMonsters.push_back(golem);
                _mCharacters.push_back(_mMonsters.back());
                break;
            }
                
            case 'S': {
                auto spectre = std::make_shared<Spectre>(50, 10, 'S');
                spectre->SetPosition(Vector2i{ x, y });
                spectre->SetGame(this);
                spectre->SetColor(GetNextColor());
                _mGameMonsters.push_back(spectre);
                _mMonsters.push_back(spectre);
                _mCharacters.push_back(_mMonsters.back());
                break;
            }
                
            case 'F': {
                auto faucheur = std::make_shared<Faucheur>(50, 30, 'F');
                faucheur->SetPosition(Vector2i{ x, y });
                faucheur->SetGame(this);
                faucheur->SetColor(GetNextColor());
                _mMonsters.push_back(faucheur);
                _mGameMonsters.push_back(faucheur);
                _mCharacters.push_back(_mMonsters.back());
                break;
            }

            case '#': {
                _mNavigator = std::make_shared<Navigator>(x, y, '#');
                _mValidMoves.push_back(Vector2i{ x, y });
                _mMap[y][x] = ' ';
                DisplayValidMoves();
                break;
            }

            }
        }
    }

    _mTurn = 0;
}

void Game::LoadMap() {
    _mMap.clear();
    std::string filename = "map/map_" + std::to_string(_mCurrentRoom) + ".txt";
    std::ifstream mapFile(filename);
    if (!mapFile.is_open()) {
        std::cerr << "Failed to open map file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(mapFile, line)) {
        _mMap.push_back(line);
    }

    mapFile.close();
}

void Game::Run() {
    while (true) {
        Render();

        if (!_mPlayer->IsDead()) {
            switch (_mTurn) {
            case 0:
                std::cout << "Your turn" << std::endl;
                HandleInput();
                CheckWinLoseCondition();
                if (_mPlayerWon) {
                    std::cout << "Congratulations! You've cleared all rooms and won the game!" << std::endl;
                    std::cout << "Press any key to restart..." << std::endl;
                    _getch();
                	_mRestartGame = true;
                }

                _mTurn = 1;
                break;
            case 1:
                for (int i = 0; i < _mMonsters.size(); i++) {
                    _mMonsters[i]->Update(_mCharacters, _mMap);
                    CheckWinLoseCondition();
                }
                _mTurn = 0;
                break;
            default:
                break;
            }
        } else {
            std::cout << "Game Over. You were defeated." << std::endl;
            std::cout << "Press any key to restart..." << std::endl;
            _getch();
            _mRestartGame = true;
        }

        if (_mRestartGame) {
            RestartGame();
            Run();
        }

	}
}


void Game::Render() {
    system("cls");

    std::vector<std::string> renderMap = _mMap;
    std::vector<std::string> statsOutput = RenderStats();

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

    std::ostringstream boardStream;

    for (size_t y = 0; y < renderMap.size(); ++y) {
        for (size_t x = 0; x < renderMap[y].length(); ++x) {
            char currentChar = renderMap[y][x];

            if (currentChar == '-') {
                boardStream << "---"; // Expand horizontal borders
            }
            else {
                bool charColored = false;

                for (const auto& character : _mCharacters) {
                    if (character->GetX() == x && character->GetY() == y) {
                        boardStream << character->GetColor() << " " << currentChar << " " << RESET;
                        charColored = true;
                        break;
                    }
                }

                if (!charColored && _mNavigator && _mCurrentState == GameState::Moving &&
                    _mNavigator->GetPosition().x == x && _mNavigator->GetPosition().y == y) {
                    boardStream << _mNavigator->GetColor() << " " << currentChar << " " << RESET;
                    charColored = true;
                }

                if (!charColored) {
                    if (currentChar == '.') {
                        boardStream << "\033[44m   " << RESET; // Blue background for valid moves
                    }
                    else {
                        boardStream << " " << (currentChar == ' ' ? ' ' : currentChar) << " ";
                    }
                }
            }
        }

        if (y < statsOutput.size()) {
            boardStream << "    " << statsOutput[y];
        }
        boardStream << "\n";
    }

    std::cout << boardStream.str();

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

    std::cout << "\nCurrent Room: " << _mCurrentRoom << "/" << _mTotalRooms << std::endl;
    std::cout << "\nControls:\n";
    if (_mCurrentState == GameState::Moving) {
        std::cout << "Arrow keys: Move navigator | Space: Move player | Enter: Attack mode | Esc: Quit\n";
    }
    else if (_mCurrentState == GameState::Attacking) {
        std::cout << "Arrow keys: Select monster | Space: Confirm attack | Esc: Exit attack mode\n";
    }
}


std::vector<std::string> Game::RenderStats() {
    std::vector<std::string> statsOutput;
    std::stringstream ss;

    ss << "Player Stats:\n";
    ss << std::setw(15) << std::left << "Name:" << _mPlayer->GetSymbol() << "\n";
    ss << std::setw(15) << std::left << "HP:" << _mPlayer->GetHp() << "/" << _mPlayer->GetMaxHp() << "\n";
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
            else if (key == 32) { // Space key
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
    if (_mNavigator) {
        Vector2i newPos = _mNavigator->GetPosition();
        if (std::find(_mValidMoves.begin(), _mValidMoves.end(), newPos) != _mValidMoves.end()) {
            Vector2i oldPos = _mPlayer->GetPosition();
            _mPlayer->SetPosition(newPos);
            AddToActionLog("Player moved from (" + std::to_string(oldPos.x) + "," +
                std::to_string(oldPos.y) + ") to (" +
                std::to_string(newPos.x) + "," +
                std::to_string(newPos.y) + ")");
            ClearValidMoves();
            CalculateValidMoves(_mPlayer->GetMoveLength());
        }
    }
}

bool Game::IsValidMove(Vector2i position) {
    if (_mMap[position.y][position.x] == '.') {
        return true;
    }
    return false;
}


void Game::MoveNavigator(int dx, int dy) {
    if (_mNavigator) {
        Vector2i newPos = { _mNavigator->GetX() + dx, _mNavigator->GetY() + dy };
        if (IsValidMove(newPos)) {
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
    ClearValidMoves();
}

void Game::ExitAttackMode() {
    _mCurrentState = GameState::Moving;
    _mAttackableMonsters.clear();
    _mSelectedMonsterIndex = 0;
    CalculateValidMoves(_mPlayer->GetMoveLength());
}

void Game::SelectNextMonster(int direction) {
    if (_mAttackableMonsters.empty()) return;
    _mSelectedMonsterIndex = (_mSelectedMonsterIndex + direction + _mAttackableMonsters.size()) % _mAttackableMonsters.size();
}

void Game::AttackMonster(std::vector<std::shared_ptr<Monster>>::const_reference monster) {
    int damage = _mPlayer->GetDamageAmount();
    monster->TakeDamage(damage, _mPlayer.get());

    AddToActionLog("Player attacked " + monster->GetColor() + std::string(1, monster->GetSymbol()) + RESET +
        " for " + std::to_string(damage) + " damage");


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

void Game::Debug(std::vector<std::string> GameMap)
{
    std::vector<std::string> tmp = _mMap;
    _mMap = GameMap;
    Render(); 
    while( _getch() != 'e');
   _mMap = tmp;
}

std::vector<std::shared_ptr<Character>> Game::GetAllMonsters()
{
    return _mGameMonsters;
}

std::shared_ptr<Player> Game::GetPlayer()
{
    return _mPlayer;
}

std::vector<std::string> Game::GetMap()
{
    return _mMap;
}

std::string Game::GetNextColor() {
    std::string color = _mColors[_mCurrentColorIndex];
    _mCurrentColorIndex = (_mCurrentColorIndex + 1) % _mColors.size();
    return color;
}

void Game::CalculateValidMoves(int moveRange) {
    _mValidMoves.clear();
    Vector2i playerPos = _mPlayer->GetPosition();

    for (int dy = -moveRange; dy <= moveRange; ++dy) {
        for (int dx = -moveRange; dx <= moveRange; ++dx) {
            if (abs(dx) + abs(dy) <= moveRange) {
                Vector2i newPos = { playerPos.x + dx, playerPos.y + dy };

                if (newPos.y >= 0 && newPos.y < _mMap.size() &&
                    newPos.x >= 0 && newPos.x < _mMap[newPos.y].length()) {

                    if (_mMap[newPos.y][newPos.x] == ' ') {
                        _mValidMoves.push_back(newPos);
                    }
                }
            }
        }
    }
    DisplayValidMoves();
}

void Game::DisplayValidMoves() {
    for (const auto& pos : _mValidMoves) {
        if (_mMap[pos.y][pos.x] == ' ') {
            _mMap[pos.y][pos.x] = '.';
        }
    }
}

void Game::ClearValidMoves() {
    for (const auto& pos : _mValidMoves) {
        if (_mMap[pos.y][pos.x] == '.') {
            _mMap[pos.y][pos.x] = ' ';
        }
    }
    _mValidMoves.clear();
}

void Game::CheckWinLoseCondition() {
    if (_mPlayer->GetHp() <= 0) {
        _mPlayer->SetHp(0);
        EndGame(false);
        return;
    }

    if (_mMonsters.empty()) {
        if (_mCurrentRoom < _mTotalRooms) {
            LoadNextRoom();
        }
        else {
            EndGame(true);
        }
    }
}

void Game::LoadNextRoom() {
    _mCurrentRoom++;

    int playerHp = _mPlayer->GetHp();
    int playerDamage = _mPlayer->GetDamageAmount();
    int playerMoveLength = _mPlayer->GetMoveLength();

    _mMonsters.clear();
    _mGameMonsters.clear();
    _mCharacters.clear();
    Init();

    _mPlayer->SetHp(playerHp);
    _mPlayer->SetDamageAmount(playerDamage);
    _mPlayer->SetMoveLength(playerMoveLength);
}

void Game::EndGame(bool playerWon) {
    _mPlayerWon = playerWon;
}


void Game::RestartGame() {
    _mCurrentRoom = 1;
    _mPlayerWon = false;
    _mRestartGame = false;

    _mMonsters.clear();
    _mGameMonsters.clear();
    _mCharacters.clear();
    _mActionLog.clear();

    Init();
}
