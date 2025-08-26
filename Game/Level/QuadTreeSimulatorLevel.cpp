#include "QuadTreeSimulatorLevel.h"
#include "Input.h"
#include "Game/Game.h"
#include "Actor/QuadPlayer.h"
#include "Actor/QuadObject.h"
#include "Actor/QuadNode.h"

QuadTreeSimulatorLevel::QuadTreeSimulatorLevel()
{
	// 일시정지 레벨 생성
	dynamic_cast<Game*>(&Game::Get())->CreatePauseLevel();

	root = new QuadNode(minX, maxX, maxY, minY, 0);
}

QuadTreeSimulatorLevel::~QuadTreeSimulatorLevel()
{
}

void QuadTreeSimulatorLevel::Tick(float deltaTime)
{
	// 스페이스바 누르면 일시정지
	if (Input::Get().GetKeyDown(VK_SPACE))
	{
		pause = !pause;
	}
	
	if (pause) return;


	super::Tick(deltaTime);

	// 삽입 중인 경우 정지
	if (isInserting) return;


	// esc 누르면 일시정지
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		dynamic_cast<Game*>(&Game::Get())->ToggleMenu();
	}

	// i 누르면 삽입
	if (Input::Get().GetKeyDown('i')|| Input::Get().GetKeyDown('I'))
	{
		insert();
	}
}

void QuadTreeSimulatorLevel::Render()
{
	if(pause) Game::Get().WriteToBuffer(Vector2::Zero, "Pause", Color::White);
	else Game::Get().WriteToBuffer(Vector2::Zero, "Quad Tree Simulator Level", Color::White);

	// 스크린 그리기
	Vector2 drawPosition(minX - 1, minY - 1);
	int width = maxX - minX + 3;
	int height = maxY - minY + 1;
	std::string screenLine(width, '#');

	Game::Get().WriteToBuffer(drawPosition, screenLine.c_str(), Color::White);
	drawPosition.y++;
	Vector2 drawPosition2(maxX + 1, drawPosition.y);
	for (int i = 0; i < height; i++)
	{
		Game::Get().WriteToBuffer(drawPosition, "#", Color::White);
		Game::Get().WriteToBuffer(drawPosition2, "#", Color::White);
		drawPosition.y++;
		drawPosition2.y++;
	}
	Game::Get().WriteToBuffer(drawPosition, screenLine.c_str(), Color::White);




	super::Render();
}

void QuadTreeSimulatorLevel::insert()
{
	AddActor(new QuadPlayer(objectId, Vector2(minX, minY)));
	isInserting = true;
}

void QuadTreeSimulatorLevel::insertFinish(int id, Vector2 position)
{
	if (id == 0)
	{
		isInserting = false;
		return;
	}

	QuadObject* newObject = new QuadObject(id, Color::White, position);
	AddActor(newObject);
	objectId++;
	isInserting = false;
}
