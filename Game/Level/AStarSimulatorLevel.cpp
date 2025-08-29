#include "AStarSimulatorLevel.h"
#include "Input.h"
#include "Game/Game.h"

#include <string>

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

	Vector2 drawPosition(leftX - 1, upY - 1);
	int width = rightX - leftX + 3;
	int height = downY - upY + 1;

	std::string screenLine(width, '#');

	Game::Get().WriteToBuffer(drawPosition, screenLine.c_str(), Color::White);
	drawPosition.y++;
	Vector2 drawPosition2(rightX + 1, drawPosition.y);

	for (int i = 0; i < height; i++)
	{
		Game::Get().WriteToBuffer(drawPosition, "#", Color::White);
		Game::Get().WriteToBuffer(drawPosition2, "#", Color::White);
		drawPosition.y++;
		drawPosition2.y++;
	}
	Game::Get().WriteToBuffer(drawPosition, screenLine.c_str(), Color::White);
}
