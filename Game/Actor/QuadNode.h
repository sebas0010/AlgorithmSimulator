#pragma once

#include "Actor/Actor.h"
#include "Actor/QuadObject.h"
#include "Utils/Timer.h"
#include <vector>

class QuadNode : public Actor
{
	RTTI_DECLARATIONS(QuadNode, Actor)

public:
	QuadNode(int xLeft, int xRight, int yDown, int yUp, int depth, Color color);

	void Tick(float deltaTime) override;
	void Render() override;

	// 삽입 함수
	void Insert(QuadObject* newObject);

	// 분할 함수
	void DevideStart();
	void DevideProcess();

	// 이펙트 함수
	void ColorChange();

	// 부모 노드 getter/setter
	QuadNode* GetParent() const { return parent; }
	void SetParent(QuadNode* p) { parent = p; }

	// 분할 발생 플래그
	int dividing = 0;
private:
	// 최대 깊이
	int maxDepth = 4;

	// 용량
	int capacity = 2;

	// 노드의 사각형 좌표
	int xLeft;
	int xRight;
	int yDown;
	int yUp;

	// 깊이
	int depth;
	// 분할 여부
	bool divided = false;

	// 이펙트 발생 시 거쳐가는 색깔
	Color colorEffect = Color::White;
	Color colorOrigin = Color::White;

	// 이펙트 발생 시 색깔 코드
	int code = 0;

	// 이펙트 발생에 쓰이는 타이머
	Timer timer;

	// 노드가 가지고 있는 오브젝트
	std::vector<QuadObject*> points;

	// 부모 노드
	QuadNode* parent = nullptr;

	// 자손 노드
	QuadNode* leftUpNode = nullptr;
	QuadNode* rightUpNode = nullptr;
	QuadNode* leftDownNode = nullptr;
	QuadNode* rightDownNode = nullptr;
};