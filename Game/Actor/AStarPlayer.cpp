#include "AStarPlayer.h"

AStarPlayer::AStarPlayer(Vector2 position)
	: Actor("P", Color::BlueIntensity, position)
{
	super::sortingOrder = 3;
	movingTimer.SetTargetTime(0.7f);
}

void AStarPlayer::BeginPlay()
{
	level = dynamic_cast<AStarSimulatorLevel*>(owner);
	hasBeganPlay = true;
}

void AStarPlayer::Tick(float deltaTime)
{
	movingTimer.Tick(deltaTime);

	if (movingTimer.IsTimeout())
	{
		if (movingRight)
		{
			if (position.x < level->GetMaxX()) position.x++;
			movingTimer.SetTargetTime(0.05f);
		}
		else if (movingLeft)
		{
			if (position.x > level->GetMinX()) position.x--;
			movingTimer.SetTargetTime(0.05f);
		}
		if (movingUp)
		{
			if (position.y > level->GetMinY()) position.y--;
			movingTimer.SetTargetTime(0.05f);
		}
		else if (movingDown)
		{
			if (position.y < level->GetMaxY()) position.y++;
			movingTimer.SetTargetTime(0.05f);
		}
		movingTimer.Reset();
	}

	if (Input::Get().GetKeyDown(VK_LEFT))
	{
		if (position.x > level->GetMinX()) position.x--;
		if (!movingLeft && !movingRight && !movingUp && !movingDown) movingTimer.SetTargetTime(0.5f);
		movingRight = false;
		movingLeft = true;

		movingTimer.Reset();
	}
	if (Input::Get().GetKeyUp(VK_LEFT))
	{
		movingLeft = false;
	}

	if (Input::Get().GetKeyDown(VK_RIGHT))
	{
		if (position.x < level->GetMaxX()) position.x++;
		if (!movingLeft && !movingRight && !movingUp && !movingDown) movingTimer.SetTargetTime(0.5f);
		movingLeft = false;
		movingRight = true;

		movingTimer.Reset();
	}
	if (Input::Get().GetKeyUp(VK_RIGHT))
	{
		movingRight = false;
	}

	if (Input::Get().GetKeyDown(VK_UP))
	{
		if (position.y > level->GetMinY()) position.y--;
		if (!movingLeft && !movingRight && !movingUp && !movingDown) movingTimer.SetTargetTime(0.5f);
		movingDown = false;
		movingUp = true;

		movingTimer.Reset();
	}
	if (Input::Get().GetKeyUp(VK_UP))
	{
		movingUp = false;
	}

	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		if (position.y < level->GetMaxY()) position.y++;
		if (!movingLeft && !movingRight && !movingUp && !movingDown) movingTimer.SetTargetTime(0.5f);
		movingUp = false;
		movingDown = true;

		movingTimer.Reset();
	}
	if (Input::Get().GetKeyUp(VK_DOWN))
	{
		movingDown = false;
	}

	// S 키를 누르면 시작 지점 지정
	if (Input::Get().GetKeyDown('s') || Input::Get().GetKeyDown('S'))
	{
		level->SetStart(position);
	}

	// G 키를 누르면 골 지점 지정
	if (Input::Get().GetKeyDown('g') || Input::Get().GetKeyDown('G'))
	{
		level->SetGoal(position);
	}

	// W 키를 누르면 벽 생성
	if (Input::Get().GetKey('w') || Input::Get().GetKey('W'))
	{
		level->SetWall(position);
	}

	// D 키를 누르면 제거
	if (Input::Get().GetKey('d') || Input::Get().GetKey('D'))
	{
		level->DeleteObject(position);
	}

	// C 키를 누르면 제거
	if (Input::Get().GetKeyDown('c') || Input::Get().GetKeyDown('C'))
	{
		level->AllClear();
	}

	// 엔터 키를 누르면 시뮬레이터 시작
	if (Input::Get().GetKeyDown(VK_RETURN))
	{
		level->Simulator();
	}
}
