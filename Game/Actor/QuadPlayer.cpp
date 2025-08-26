#include "QuadPlayer.h"
#include "Input.h"

QuadPlayer::QuadPlayer(int id, Vector2 position)
	: Actor(std::to_string(id).c_str(), Color::White, position), id(id)
{
	
	timer.Reset();
	timer.SetTargetTime(0.3f);

	movingTimer.SetTargetTime(0.7f);
}

void QuadPlayer::BeginPlay()
{
	level = dynamic_cast<QuadTreeSimulatorLevel*>(owner);
	hasBeganPlay = true;
}

void QuadPlayer::Tick(float deltaTime)
{
	timer.Tick(deltaTime);
	movingTimer.Tick(deltaTime);

	if (timer.IsTimeout())
	{
		timer.Reset();
		ChangeColor();
	}


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
		if(position.x > level->GetMinX()) position.x--;
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


	// 엔터키를 누르면 해당 자리에 오브젝트 생성
	if (Input::Get().GetKeyDown(VK_RETURN))
	{
		level->insertFinish(id, position);
		Destroy();
	}
	// ESC키를 누르면 생성 취소
	if (Input::Get().GetKeyUp(VK_ESCAPE))
	{
		level->insertFinish(0, position);
		Destroy();
	}
}

void QuadPlayer::ChangeColor()
{
	switch (code)
	{
		case 0:
			color = Color::RedIntensity;
			code = 1;
			break;
		case 1:
			color = Color::Red;
			code = 2;
			break;
		case 2:
			color = Color::RedIntensity;
			code = 3;
			break;
		case 3:
			color = Color::White;
			code = 0;
			break;
	}
}
