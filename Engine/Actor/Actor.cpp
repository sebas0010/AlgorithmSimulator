#include "Actor.h"
#include "Utils/Utils.h"
#include "Engine.h"
#include "Level/Level.h"

#include <Windows.h>
#include <iostream>

Actor::Actor(const char* image, Color color, const Vector2& position)
	: color(color), position(position)
{
	// 문자열 길이.
	width = (int)strlen(image);

	// 메모리 할당.
	this->image = new char[width + 1];

	// 문자열 복사.
	strcpy_s(this->image, width + 1, image);
}

Actor::~Actor()
{
	// 메모리 해제.
	SafeDeleteArray(image);
}

// 객체 생애주기(Lifetime)에 1번만 호출됨 (초기화 목적).
void Actor::BeginPlay()
{
	hasBeganPlay = true;
}

// 프레임 마다 호출 (반복성 작업/지속성이 필요한 작업).
void Actor::Tick(float deltaTime)
{
}

// 그리기 함수.
void Actor::Render()
{
	Engine::Get().WriteToBuffer(position, image, color);
}

void Actor::SetPosition(const Vector2& newPosition)
{
	// 같으면 업데이트 안함.
	if (position == newPosition)
	{
		return;
	}

	position = newPosition;
}

Vector2 Actor::Position() const
{
	return position;
}

int Actor::Width() const
{
	return width;
}

void Actor::SetSortingOrder(unsigned int sortingOrder)
{
	this->sortingOrder = sortingOrder;
}

void Actor::SetOwner(Level* newOwner)
{
	owner = newOwner;
}

Level* Actor::GetOwner()
{
	return owner;
}

void Actor::Destroy()
{
	// 중복 삭제 방지 처리.
	if (isExpired)
	{
		return;
	}

	// 삭제 요청 되었다고 설정.
	isExpired = true;

	// 레벨에 삭제 요청.
	owner->DestroyActor(this);
}

void Actor::QuitGame()
{
	Engine::Get().Quit();
}