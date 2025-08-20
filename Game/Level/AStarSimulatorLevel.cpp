#include "AStarSimulatorLevel.h"
#include "Input.h"
#include "Game/Game.h"

AStarSimulatorLevel::AStarSimulatorLevel()
{
	// 일시정지 레벨 생성
	dynamic_cast<Game*>(&Game::Get())->CreatePauseLevel();
}

AStarSimulatorLevel::~AStarSimulatorLevel()
{
}

void AStarSimulatorLevel::Tick(float deltaTime)
{
	// esc 누르면 일시정지
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		dynamic_cast<Game*>(&Game::Get())->ToggleMenu();
	}
}

void AStarSimulatorLevel::Render()
{
	Game::Get().WriteToBuffer(Vector2::Zero, "A Star Simulator Level", Color::White);
}
