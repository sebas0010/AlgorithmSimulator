#include "Game.h"
#include "Level/MainMenuLevel.h"

Game::Game()
{
	AddLevel(new MainMenuLevel());
}
