#include "BSTSimulatorLevel.h"
#include "Input.h"
#include "Game/Game.h"

BSTSimulatorLevel::BSTSimulatorLevel()
{
	dynamic_cast<Game*>(&Game::Get())->CreatePauseLevel();
}

BSTSimulatorLevel::~BSTSimulatorLevel()
{
}

void BSTSimulatorLevel::Tick(float deltaTime)
{
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		dynamic_cast<Game*>(&Game::Get())->ToggleMenu();
	}
}

void BSTSimulatorLevel::Render()
{
	Game::Get().WriteToBuffer(Vector2::Zero, "Binary Search Tree Simulator Level", Color::White);
}
