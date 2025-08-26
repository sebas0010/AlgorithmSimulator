#include "QuadNode.h"
#include "Game/Game.h"
#include "Level/QuadTreeSimulatorLevel.h"

QuadNode::QuadNode(int xLeft, int xRight, int yDown, int yUp, int depth, Color color)
	: xLeft(xLeft), xRight(xRight), yDown(yDown), yUp(yUp), depth(depth)
{
	super::color = color;
	colorOrigin = color;
	timer.Reset();
	timer.SetTargetTime(0.4f);
}

void QuadNode::Tick(float deltaTime)
{
	if (dividing > 0)
	{
		if (code == 7)
		{
			code = 0;
			DevideProcess();
			dividing = 0;
			return;
		}
		timer.Tick(deltaTime);
		if (timer.IsTimeout())
		{
			timer.Reset();
			ColorChange();
		}
	}
}

void QuadNode::Render()
{
	Vector2 drawPosition(xLeft, yUp);
	int width = xRight - xLeft + 1;
	int height = yDown - yUp + 1;
	std::string screenLine(width, '#');

	Game::Get().WriteToBuffer(drawPosition, screenLine.c_str(), color);
	drawPosition.y++;
	Vector2 drawPosition2(xRight, drawPosition.y);
	for (int i = 0; i < height-2; i++)
	{
		Game::Get().WriteToBuffer(drawPosition, "#", color);
		Game::Get().WriteToBuffer(drawPosition2, "#", color);
		drawPosition.y++;
		drawPosition2.y++;
	}
	drawPosition.y = yDown;
	Game::Get().WriteToBuffer(drawPosition, screenLine.c_str(), color);
}

void QuadNode::Insert(QuadObject* newObject)
{
	// 이미 분할된 노드라면 하위 노드에 삽입
	if (divided)
	{
		int midX = (xLeft + xRight - 1) / 2;
		int midY = (yDown + yUp - 1) / 2;

		Vector2 p = newObject->Position();

		bool isLeft = (p.x <= midX);
		bool isUp = (p.y <= midY);

		// 좌표에 맞게 노드 삽입
		if (isLeft && isUp) leftUpNode->Insert(newObject);
		else if (!isLeft && isUp) rightUpNode->Insert(newObject);
		else if (isLeft && !isUp) leftDownNode->Insert(newObject);
		else rightDownNode->Insert(newObject);

		return;
	}

	points.emplace_back(newObject);

	if (depth != maxDepth && points.size() > capacity)
	{
		DevideStart();
	}
}

void QuadNode::DevideStart()
{
	dividing++;
}

void QuadNode::DevideProcess()
{
	int midX = (xLeft + xRight - 1) / 2;
	int midY = (yDown + yUp - 1) / 2;

	leftDownNode = new QuadNode(xLeft, midX, yDown, midY + 1, depth + 1, Color::RedIntensity);
	rightDownNode = new QuadNode(midX + 1, xRight, yDown, midY + 1, depth + 1, Color::BlueIntensity);
	leftUpNode = new QuadNode(xLeft, midX, midY, yUp, depth + 1, Color::GreenIntensity);
	rightUpNode = new QuadNode(midX + 1, xRight, midY, yUp, depth + 1, Color::YellowIntensity);

	owner->AddActor(leftDownNode);
	owner->AddActor(rightDownNode);
	owner->AddActor(leftUpNode);
	owner->AddActor(rightUpNode);

	leftDownNode->SetParent(this);
	rightDownNode->SetParent(this);
	leftUpNode->SetParent(this);
	rightUpNode->SetParent(this);

	while (points.size() != 0)
	{
		QuadObject* object = points.back();
		points.pop_back();

		Vector2 p = object->Position();

		bool isLeft = (p.x <= midX);
		bool isUp = (p.y <= midY);

		// 좌표에 맞게 노드 삽입
		if (isLeft && isUp) leftUpNode->Insert(object);
		else if (!isLeft && isUp) rightUpNode->Insert(object);
		else if (isLeft && !isUp) leftDownNode->Insert(object);
		else rightDownNode->Insert(object);
	}

	divided = true;
}

void QuadNode::ColorChange()
{
	if (code % 2 == 0)
	{
		color = colorEffect;
		code++;
	}
	else
	{
		color = colorOrigin;
		code++;
	}
}
