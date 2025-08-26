#pragma once

#include "Actor/Actor.h"
#include "Input.h"
#include "Utils/Timer.h"
#include "Level/QuadTreeSimulatorLevel.h"

class QuadPlayer : public Actor
{
	RTTI_DECLARATIONS(QuadPlayer,  Actor)

public:
	QuadPlayer(int id, Vector2 position);

	void BeginPlay() override;
	void Tick(float deltaTime) override;

private:
	// 깜빡이기 위한 색깔 코드와 색깔 전환 함수와 타이머
	void ChangeColor();
	int code = 0;
	Timer timer;

	// 플레이어 액터 소유 레벨
	QuadTreeSimulatorLevel* level = nullptr;

	// 배치할 오브젝트의 숫자
	int id;

	// 이동 상태 변수
	bool movingLeft = false;
	bool movingRight = false;
	bool movingUp = false;
	bool movingDown = false;
	Timer movingTimer;
};