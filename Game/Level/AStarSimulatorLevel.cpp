#include "AStarSimulatorLevel.h"
#include "Input.h"
#include "Game/Game.h"

AStarSimulatorLevel::AStarSimulatorLevel()
{
	dynamic_cast<Game*>(&Game::Get())->CreatePauseLevel();
}

AStarSimulatorLevel::~AStarSimulatorLevel()
{
}

void AStarSimulatorLevel::Tick(float deltaTime)
{
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		dynamic_cast<Game*>(&Game::Get())->ToggleMenu();
	}
}

void AStarSimulatorLevel::Render()
{
	Game::Get().WriteToBuffer(Vector2::Zero, "A Star Simulator Level", Color::White);
}
