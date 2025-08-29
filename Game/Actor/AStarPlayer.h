#pragma once

#include "Actor/Actor.h"
#include "Input.h"
#include "Utils/Timer.h"
#include "Level/AStarSimulatorLevel.h"

class AStarPlayer : public Actor
{
	RTTI_DECLARATIONS(AStarPlayer, Actor)
public:
	AStarPlayer(Vector2 position);

	void BeginPlay() override;
	void Tick(float deltaTime) override;

private:
	AStarSimulatorLevel* level = nullptr;

	// 움직임을 위한 타이머
	Timer movingTimer;

	// 이동 상태 변수
	bool movingLeft = false;
	bool movingRight = false;
	bool movingUp = false;
	bool movingDown = false;
};