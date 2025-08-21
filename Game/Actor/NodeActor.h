#pragma once

#include "Actor/Actor.h"
#include "Utils/Timer.h"

class NodeActor : public Actor
{ 
	RTTI_DECLARATIONS(NodeActor, Actor)

public:
	NodeActor(int data, Vector2 position, NodeActor* parent = nullptr);
	void Tick(float deltaTime) override;
	void Render() override;

	// 색깔 코드에 맞게 색깔 바꾸는 함수
	void ColorChange();

	// 깜빡이는 신호
	void Blink();

	// 부모 자식 노드 Getter/Setter
	const NodeActor* GetParent() const { return parent; }
	void SetParent(NodeActor* newParent) { parent = newParent; }

	const NodeActor* GetLeft() const { return left; }
	void SetLeft(NodeActor* newLeft) { left = newLeft; }

	const NodeActor* GetRight() const { return right; }
	void SetRight(NodeActor* newRight) { right = newRight; }

private:

	// 부모
	NodeActor* parent = nullptr;

	// 왼쪽 자손
	NodeActor* left = nullptr;

	// 오른쪽 자손
	NodeActor* right = nullptr;

	// 데이터
	int data;

	// 깜빡이기 위한 플래그
	int blink = 0;

	// 색깔 코드
	int code = 0;

	// 깜빡이는 효과를 주기 위한 타이머
	Timer timer;
};