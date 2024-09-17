#include <iostream>

#include "Game.h"

int main() {
	const auto game = std::make_shared<Game>();
	game->Init();
	game->Run();

}
