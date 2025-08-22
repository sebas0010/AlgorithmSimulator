#pragma once

#include "Level/Level.h"
#include "Actor/NodeActor.h"

class AVLTreeSimulator : public Level
{
	RTTI_DECLARATIONS(AVLTreeSimulator, Level)

public:
	AVLTreeSimulator();
	~AVLTreeSimulator();

	void Tick(float deltaTime) override;
	void Render() override;

	// 노드 추가를 위한 입력을 받는 함수
	void GetInput();

	// 트리 위치를 위한 함수
	void LocateTree();

	// 트리 위치 함수에서 재귀적으로 호출할 함수
	void LocateTreeRecursive(NodeActor* node, int height, Vector2 nodePosition);

	// 트리의 높이를 구하기 위한 함수
	int TreeHeight(NodeActor* node);

	// 트리에 노드 삽입
	void InsertNode(int data);

	// 트리에 노드 삽입 신호를 주는 함수
	void StartInsertNode(int data);

	// 트리에 노드 삽입하는 함수
	void InsertNodeProcessing();

	// 삽입 직후에 불균형을 찾는 함수
	void MarkFirstUnbalancedTriplet(NodeActor* inserted);

	// 
	void ReplaceParent(NodeActor* z, NodeActor* newRoot);

	void RotateRight(NodeActor* z);

	void RotateLeft(NodeActor* z);

	void RotateLeftRight(NodeActor* z);

	void RotateRightLeft(NodeActor* z);

	void DoRotation();

private:

	// AVL 트리의 뿌리
	NodeActor* treeRoot = nullptr;

	// AVL 트리의 노드 숫자
	int nodeCount = 0;

	// 노드 추가 단계 플래그
	bool isInserting = false;

	// 추가 중인 노드 포인터
	NodeActor* addActor = nullptr;

	// 추가 중인 노드가 비교하고 있는 노드의 포인터
	NodeActor* curActor = nullptr;

	// 회전 단계 플래그
	bool isRotating = false;

	// 회전 단계 노드 포인터
	NodeActor* rotateNodeX = nullptr;
	NodeActor* rotateNodeY = nullptr;
	NodeActor* rotateNodeZ = nullptr;

	// 단계별 작동을 위한 타이머
	Timer timer;
};