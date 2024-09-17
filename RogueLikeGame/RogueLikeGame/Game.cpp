#include "Game.h"
#include <fstream>
#include <iostream>
#include <conio.h>

#include "Faucheur.h"
#include "Golem.h"
#include "Navigator.h"
#include "Spectre.h"

Game::Game() {}

Game::~Game() {}

void Game::Init() {
    LoadMap();
    for (size_t x = 0; x < _mMap.size(); ++x) {
        for (size_t y = 0; y < _mMap[x].length(); ++y) {
            switch (_mMap[x][y]) {
            case '@': {
                _mPlayer = std::make_shared<Player>(x, y);
                _mCharacters.push_back(_mPlayer);
                break;
            }
               
            case 'G': {
                auto golem = std::make_shared<Golem>();
                _mMonsters.push_back(golem);
                _mCharacters.push_back(_mMonsters.back());
                break;
            }
                
            case 'S': {
                auto spectre = std::make_shared<Spectre>();
                _mMonsters.push_back(spectre);
                _mCharacters.push_back(_mMonsters.back());
                break;
            }
                
            case 'F': {
                auto faucheur = std::make_shared<Faucheur>();
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
        HandleInput();
    }
}

void Game::Render() {
    system("cls");

    std::vector<std::string> renderMap = _mMap;


    // Add navigator to the render map
    if (_mNavigator) {
        int x = _mNavigator->GetX();
        int y = _mNavigator->GetY();
        if (y >= 0 && y < renderMap.size() && x >= 0 && x < renderMap[y].length()) {
            renderMap[y][x] = _mNavigator->GetSymbol();
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

    if (_kbhit()) {
        int key = _getch();
        int dx = 0, dy = 0;

        if (key == 224) { // Arrow key pressed
            key = _getch(); // Get the actual arrow key code
            switch (key) {
            case 72: dy = -1; break; // Up arrow
            case 80: dy = 1; break;  // Down arrow
            case 75: dx = -1; break; // Left arrow
            case 77: dx = 1; break;  // Right arrow
            }
        }
        else {
            switch (key) {
            case 13: // Enter key
                if (_mNavigator) {
                    Move();
                }
                break;
            case 27: // Esc key
                exit(0);
                break;
            }
        }

        if (dx != 0 || dy != 0) {
            MoveNavigator(dx, dy);
        }
    }
}

void Game::Move() {
    if (_mNavigator && IsValidMove(_mNavigator->GetX(), _mNavigator->GetY())) {
        _mPlayer->SetPosition(_mNavigator->GetPosition());
    }
}

bool Game::IsValidMove(int x, int y) {
    return true;
}

void Game::MoveNavigator(int dx, int dy) {
    if (_mNavigator) {
	    const int newX = _mNavigator->GetX() + dx;
	    const int newY = _mNavigator->GetY() + dy;
        if (IsValidMove(newX, newY)) {
            _mNavigator->Move(dx, dy);
        }
    }
}
