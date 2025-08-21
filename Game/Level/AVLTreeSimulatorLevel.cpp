#include "AVLTreeSimulatorLevel.h"
#include "Input.h"
#include "Game/Game.h"
#include "Actor/NodeActor.h"
#include "Utils/Utils.h"

#include <iostream>
#include <limits>
#include <math.h>
#include <string>


AVLTreeSimulator::AVLTreeSimulator()
{
	// 일시정지 레벨 생성
	dynamic_cast<Game*>(&Game::Get())->CreatePauseLevel();
}

AVLTreeSimulator::~AVLTreeSimulator()
{
}

void AVLTreeSimulator::Tick(float deltaTime)
{
	super::Tick(deltaTime);
	// esc 누르면 일시정지
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		dynamic_cast<Game*>(&Game::Get())->ToggleMenu();
	}

	// 
	if (Input::Get().GetKeyDown('I') || Input::Get().GetKeyDown('i'))
	{
		// 명령 받는 동안 엔진 일시정지
		Game::Get().Pause();

		// 숫자를 입력 받고 노드 추가
		GetInput();
	
		// 엔진 동작
		Game::Get().Resume();
	}
}

void AVLTreeSimulator::Render()
{
	super::Render();
	Game::Get().WriteToBuffer(Vector2::Zero, "AVL Tree Simulator", Color::BlueIntensity);

	Game::Get().WriteToBuffer(Vector2(0, 3), "I : 노드 추가, D : 노드 삭제, C : 모든 노드 삭제", Color::White);


}

void AVLTreeSimulator::GetInput()
{

	// 커서 위치 값 생성 및 커서 이동
	COORD coord{ 0, 4 };
	Utils::SetConsolePosition(coord);

	// 입력 받음
	std::string inputData;
	std::cin >> inputData;


	try {
		int data = std::stoi(inputData);
		AddActor(new NodeActor(data, Vector2(0, 5)));
	}
	catch (...) {
		// 잘못된 입력이므로 아무것도 하지 않음
	}
}
