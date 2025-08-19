#include "Game.h"
#include "Level/MainMenuLevel.h"
#include "Level/PauseLevel.h"

Game::Game()
{
	AddLevel(new MainMenuLevel());
}

Game::~Game()
{
	CleanUp();
}

void Game::CleanUp()
{
	SafeDelete(mainLevel);
	SafeDelete(backLevel);
}

void Game::CreatePauseLevel()
{
	backLevel = new PauseLevel();
}

void Game::ToggleMenu()
{
	Level* temp = mainLevel;
	mainLevel = backLevel;
	backLevel = temp;
}
