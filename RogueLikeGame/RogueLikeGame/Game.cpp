#include "Game.h"
#include <fstream>
#include <iostream>
#include <conio.h>

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
                auto golem = std::make_shared<Golem>(100, 20, 'G', 10);
                golem->SetPosition(Vector2i{ x, y });
                golem->SetGame(this);
                _mMonsters.push_back(golem);
                _mCharacters.push_back(_mMonsters.back());
                break;
            }
                
            case 'S': {
                auto spectre = std::make_shared<Spectre>(100, 10, 'S');
                spectre->SetPosition(Vector2i{ x, y });
                spectre->SetGame(this);
                _mMonsters.push_back(spectre);
                _mCharacters.push_back(_mMonsters.back());
                break;
            }
                
            case 'F': {
                auto faucheur = std::make_shared<Faucheur>(100, 30, 'F');
                faucheur->SetPosition(Vector2i{ x, y });
                faucheur->SetGame(this);
                _mMonsters.push_back(faucheur);
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
	while (true) {
        Render();
        switch (_mTurn)
        {
        case 0:
            HandleInput();
            _mTurn = 1;
            break;
        case 1:
            std::cout << "AI turn" << std::endl;
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

    // Add navigator to the render map
    if (_mNavigator) {
        Vector2i position = _mNavigator->GetPosition();
        if (IsValidMove(position)) {
            renderMap[position.y][position.x] = _mNavigator->GetSymbol();
        }
    }

    // Add player to the render map
    if (_mPlayer) {
        Vector2i position = _mPlayer->GetPosition();
        if (IsValidMove(position)) {
            renderMap[position.y][position.x] = _mPlayer->GetSymbol();
        }
    }

    // Display the updated map
    for (const auto& line : renderMap) {
        std::cout << line << std::endl;
    }

    // Display additional game information
    if (_mNavigator) {
        std::cout << "Navigator position: (" << _mNavigator->GetX() << ", " << _mNavigator->GetY() << ")" << std::endl;
    }

    // Display controls
    std::cout << "\nControls:" << std::endl;
    std::cout << "Arrow keys: Move navigator" << std::endl;
    std::cout << "Enter: Move player to navigator" << std::endl;
    std::cout << "Esc: Quit game" << std::endl;
}

void Game::HandleInput() {
    int key = _getch();
    int dx = 0, dy = 0;
    while (key != 13 && key != 32) {
        dx = 0, dy = 0;
        if (key == 224) { // Arrow key pressed
            key = _getch(); // Get the actual arrow key code
            switch (key) {
            case 72: dy = -1; break; // Up arrow
            case 80: dy = 1; break;  // Down arrow
            case 75: dx = -1; break; // Left arrow
            case 77: dx = 1; break;  // Right arrow
            }
        }
        if (key == 27) // Esc key
            exit(0);
        if (dx != 0 || dy != 0)
            MoveNavigator(dx, dy);
        Render();
        key = _getch();
        
    }

        
    if (key == 32 && _mNavigator) {
        MoveNavigator(dx, dy);
        Move();
    }
}

void Game::Move() {
    if (_mNavigator && IsValidMove(_mNavigator->GetPosition())) {
        _mPlayer->SetPosition(_mNavigator->GetPosition());
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

std::vector<std::string> Game::UpdateCharacterPositionInMap(Character *Target, Vector2i PrevPosition)
{
    Vector2i position = Target->GetPosition();
    _mMap[position.y][position.x] = Target->GetSymbol();
    _mMap[PrevPosition.y][PrevPosition.x] = ' ';
    Render();
    return _mMap;
}
