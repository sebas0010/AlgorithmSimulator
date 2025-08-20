#pragma once

#include "Engine.h"
#include "Level/Level.h"

class Game : public Engine
{
public:

	Game();
	~Game();

	// 메모리 정리 함수
	void CleanUp() override;

	void CreatePauseLevel();
	void ToggleMenu();
	void ReturnToMainMenu();

private:
	Level* backLevel = nullptr;
};