#include "AVLNode.h"
#include "Game/Game.h"

AVLNode::AVLNode(int data, Vector2 position)
	: data(data)
{
	super::position = position;
	color = Color::GreenIntensity;
	timer.Reset();
	timer.SetTargetTime(0.4f);

	blinkTimer.Reset();
	blinkTimer.SetTargetTime(0.4f);
}

void AVLNode::Tick(float deltaTime)
{
	if (blink)
	{
		timer.Tick(deltaTime);
		if (timer.IsTimeout())
		{
			timer.Reset();
			if (color == Color::White) color = Color::RedIntensity;
			else color = Color::White;
			blinkCount++;
		}
		if (blinkCount == 6)
		{
			color = Color::White;
			blink = false;
			blinkCount = 0;
			visible = false;
		}
	}
	
	if (isMoving)
	{
		Vector2 dir = targetPosition - position; // 방향 벡터
		float dist = sqrtf((float)dir.x * dir.x + (float)dir.y * dir.y);

		if (dist > 0.0f)
		{
			float step = moveSpeed * deltaTime;

			// 정규화
			float nx = dir.x / dist;
			float ny = dir.y / dist;

			// 이동 (float 누적값 갱신)
			xLocation += nx * step;
			yLocation += ny * step;

			// 최종 정수 좌표 반영
			position.x = (int)std::round(xLocation);
			position.y = (int)std::round(yLocation);
		}
		else
		{
			timer.Tick(deltaTime);
			if (timer.IsTimeout())
			{
				timer.Reset();
				isMoving = false;
			}
		}
	}
}

void AVLNode::Render()
{
	if (!visible) return;

	Vector2 drawPosition;

	// 숫자 그리기
	// 두 자릿수
	if (data / 10 > 0)
	{
		int num1 = data / 10;
		int num2 = data % 10;
		if (num1 == 1 && num2 == 1) drawPosition = Vector2(position.x - 2, position.y - 2);
		else if (num1 == 1 || num2 == 1) drawPosition = Vector2(position.x - 3, position.y - 2);
		else drawPosition = Vector2(position.x - 5, position.y - 2);

		for (int i = 0; i < 5; i++)
		{
			Game::Get().WriteToBuffer(drawPosition, bigNumber[num1][i].c_str(), color);
			drawPosition.y++;
		}

		drawPosition.y -= 5;

		if (num1 == 1 && num2 == 1) drawPosition.x += 4;
		else if (num1 == 1) drawPosition.x += 2;
		else drawPosition.x += 6;

		for (int i = 0; i < 5; i++)
		{
			Game::Get().WriteToBuffer(drawPosition, bigNumber[num2][i].c_str(), color);
			drawPosition.y++;
		}
	}
	// 한 자릿수
	else if (data / 10 == 0)
	{
		int num = data % 10;
		if(num == 1) drawPosition = Vector2(position.x, position.y - 2);
		else drawPosition = Vector2(position.x - 2, position.y - 2);

		for (int i = 0; i < 5; i++)
		{
			Game::Get().WriteToBuffer(drawPosition, bigNumber[num][i].c_str(), color);
			drawPosition.y++;
		}
	}

	// 원 그리기
	drawPosition = Vector2(position.x - 3, position.y - 5);
	Game::Get().WriteToBuffer(drawPosition, "#######", color);
	drawPosition.y += 10;
	Game::Get().WriteToBuffer(drawPosition, "#######", color);

	drawPosition = Vector2(position.x - 6, position.y - 4);
	Game::Get().WriteToBuffer(drawPosition, "###", color);
	drawPosition.x += 10;
	Game::Get().WriteToBuffer(drawPosition, "###", color);
	drawPosition.y += 8;
	Game::Get().WriteToBuffer(drawPosition, "###", color);
	drawPosition.x -= 10;
	Game::Get().WriteToBuffer(drawPosition, "###", color);

	drawPosition = Vector2(position.x - 8, position.y - 3);
	Game::Get().WriteToBuffer(drawPosition, "##", color);
	drawPosition.x += 15;
	Game::Get().WriteToBuffer(drawPosition, "##", color);
	drawPosition.y += 6;
	Game::Get().WriteToBuffer(drawPosition, "##", color);
	drawPosition.x -= 15;
	Game::Get().WriteToBuffer(drawPosition, "##", color);

	drawPosition = Vector2(position.x - 9, position.y - 2);
	Game::Get().WriteToBuffer(drawPosition, "#", color);
	drawPosition.x += 18;
	Game::Get().WriteToBuffer(drawPosition, "#", color);
	drawPosition.y += 4;
	Game::Get().WriteToBuffer(drawPosition, "#", color);
	drawPosition.x -= 18;
	Game::Get().WriteToBuffer(drawPosition, "#", color);

	drawPosition = Vector2(position.x - 10, position.y - 1);
	Game::Get().WriteToBuffer(drawPosition, "#", color);
	drawPosition.x += 20;
	Game::Get().WriteToBuffer(drawPosition, "#", color);
	drawPosition.y ++;
	Game::Get().WriteToBuffer(drawPosition, "#", color);
	drawPosition.x -= 20;
	Game::Get().WriteToBuffer(drawPosition, "#", color);
	drawPosition.y++;
	Game::Get().WriteToBuffer(drawPosition, "#", color);
	drawPosition.x += 20;
	Game::Get().WriteToBuffer(drawPosition, "#", color);

	

	// 부모와 연결하는 선 그리기
	if (parent == nullptr || !parent->visible) return;

	int x0 = parent->Position().x;
	int y0 = parent->Position().y;
	int x1 = position.x;
	int y1 = position.y;

	int dx = abs(x1 - x0);
	int sx = (x0 < x1) ? 1 : -1;
	int dy = -abs(y1 - y0);
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx + dy;

	while (true)
	{
		bool inChild = IsInsideEllipse(x0, y0, position, 10, 5, -1.0f);
		bool inParent = IsInsideEllipse(x0, y0, parent->Position(), 10, 5, -1.0f);
		if (!inChild && !inParent)
		{
			Game::Get().WriteToBuffer(Vector2(x0, y0), "*", Color::White);
		}
		if (x0 == x1 && y0 == y1) break;

		int e2 = err << 1;      // 2*err
		if (e2 >= dy) { err += dy; x0 += sx; }
		if (e2 <= dx) { err += dx; y0 += sy; }
	}
}

void AVLNode::MoveStart(Vector2 targetPosition, float speed)
{
	moveSpeed = speed;
	xLocation = (float)position.x;
	yLocation = (float)position.y;
	this->targetPosition = targetPosition;
	isMoving = true;
}

void AVLNode::Blink()
{
	blink = true;
}
