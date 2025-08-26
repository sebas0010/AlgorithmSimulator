#pragma once

#include "Actor/Actor.h"
#include "Actor/QuadObject.h"
#include <vector>

class QuadNode : public Actor
{
	RTTI_DECLARATIONS(QuadNode, Actor)

public:
	QuadNode(int xLeft, int xRight, int yDown, int yUp, int depth);

	void Tick(float deltaTime) override;
	void Render() override;


	// 부모 노드 getter/setter
	QuadNode* GetParent() const { return parent; }
	void SetParent(QuadNode* p) { parent = p; }
private:
	// 노드의 사각형 좌표
	int xLeft;
	int xRight;
	int yDown;
	int yUp;

	// 깊이
	int depth;
	// 분할 여부
	bool divided = false;


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