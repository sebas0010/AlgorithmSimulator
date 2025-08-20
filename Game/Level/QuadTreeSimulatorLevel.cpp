#include "QuadTreeSimulatorLevel.h"
#include "Input.h"
#include "Game/Game.h"

QuadTreeSimulatorLevel::QuadTreeSimulatorLevel()
{
	// 일시정지 레벨 생성
	dynamic_cast<Game*>(&Game::Get())->CreatePauseLevel();
}

QuadTreeSimulatorLevel::~QuadTreeSimulatorLevel()
{
}

void QuadTreeSimulatorLevel::Tick(float deltaTime)
{
	// esc 누르면 일시정지
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		dynamic_cast<Game*>(&Game::Get())->ToggleMenu();
	}
}

void QuadTreeSimulatorLevel::Render()
{
	Game::Get().WriteToBuffer(Vector2::Zero, "Quad Tree Simulator Level", Color::White);
}
