#include "QuadNode.h"
#include "Game/Game.h"
#include "Level/QuadTreeSimulatorLevel.h"

QuadNode::QuadNode(int xLeft, int xRight, int yDown, int yUp, int depth)
	: xLeft(xLeft), xRight(xRight), yDown(yDown), yUp(yUp), depth(depth)
{
	timer.Reset();
	timer.SetTargetTime(0.2f);
}

void QuadNode::Tick(float deltaTime)
{
	if (dividing > 0)
	{
		if (code == 12)
		{
			code = 0;
			DevideProcess();
			dividing = 0;
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
	int midY = (yDown + yUp -1) / 2;

	leftDownNode = new QuadNode(xLeft, midX, yDown, midY, depth + 1);
	rightDownNode = new QuadNode(midX + 1, xRight, yDown, midY, depth + 1);
	leftUpNode = new QuadNode(xLeft, midX, midY + 1, yUp, depth + 1);
	rightUpNode = new QuadNode(midX + 1, xRight, midY + 1, yUp, depth + 1);

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
	switch (code % 4)
	{
	case 0:
		color = colorIntensity;
		code++;
		break;
	case 1:
		color = colorEffect;
		code++;
		break;
	case 2:
		color = colorIntensity;
		code++;
		break;
	case 3:
		color = colorOrigin;
		code++;
		break;
	}
}
