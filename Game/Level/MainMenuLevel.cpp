#include "MainMenuLevel.h"
#include "Game/Game.h"
#include "AStarSimulatorLevel.h"
#include "AVLTreeSimulatorLevel.h"
#include "QuadTreeSimulatorLevel.h"

MainMenuLevel::MainMenuLevel()
{
	// 메뉴 아이템 추가 -> A* 시뮬레이터, 쿼드트리 시뮬레이터, 이진 탐색 트리 시뮬레이터, 프로그램 종료
	items.emplace_back(new MenuItem(
		"A Star Simulator",
		[]() { Game::Get().AddLevel(new AStarSimulatorLevel()); }
	));
	items.emplace_back(new MenuItem(
		"Quad Tree Simulator",
		[]() { Game::Get().AddLevel(new QuadTreeSimulatorLevel()); }
	));
	items.emplace_back(new MenuItem(
		"AVL Tree Simulator",
		[]() { Game::Get().AddLevel(new AVLTreeSimulator()); }
	));
	items.emplace_back(new MenuItem(
		"Quit",
		[]() { Game::Get().Quit(); }
	));
	// 아이템 수 미리 저장
	length = static_cast<int>(items.size());
}

MainMenuLevel::~MainMenuLevel()
{
	for (MenuItem* item : items)
	{
		SafeDelete(item);
	}

	items.clear();
}

void MainMenuLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 입력 처리
	if (Input::Get().GetKeyDown(VK_UP))
	{
		if (currentIndex <= 0) return;
		currentIndex--;
	}
	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		if (currentIndex >= length - 1) return;
		currentIndex++;
	}

	// Enter 키 입력
	if (Input::Get().GetKeyDown(VK_RETURN))
	{
		// 메뉴 아이템이 저장하고 있는 함수 호출
		items[currentIndex]->onSelected();
		currentIndex = 0;
	}
}

void MainMenuLevel::Render()
{
	Vector2 showPosition(0, 0);
	Color color = Color::Blue;
	Game::Get().WriteToBuffer(showPosition, "Simulator", color);

	showPosition.y += 2;
	for (int ix = 0; ix < length; ++ix)
	{
		color = ix == currentIndex ? selectedColor : unselectedColor;
		Game::Get().WriteToBuffer(showPosition, items[ix]->menuText, color);
		showPosition.y++;
	}
}
