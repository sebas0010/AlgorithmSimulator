#include "BSTSimulatorLevel.h"
#include "Input.h"
#include "Game/Game.h"

BSTSimulatorLevel::BSTSimulatorLevel()
{
	// 일시정지 레벨 생성
	dynamic_cast<Game*>(&Game::Get())->CreatePauseLevel();
}

BSTSimulatorLevel::~BSTSimulatorLevel()
{
}

void BSTSimulatorLevel::Tick(float deltaTime)
{
	// esc 누르면 일시정지
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		dynamic_cast<Game*>(&Game::Get())->ToggleMenu();
	}
}

void BSTSimulatorLevel::Render()
{
	Game::Get().WriteToBuffer(Vector2::Zero, "Binary Search Tree Simulator Level", Color::White);
}
