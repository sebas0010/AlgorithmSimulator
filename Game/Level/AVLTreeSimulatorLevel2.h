#pragma once

#include "Level/Level.h"
#include "Actor/AVLNode.h"

class AVLTreeSimulator2 : public Level
{
	RTTI_DECLARATIONS(AVLTreeSimulator2, Level)

public:
	AVLTreeSimulator2();
	~AVLTreeSimulator2();

	void Tick(float deltaTime) override;
	void Render() override;

	// 노드 추가 함수
	void InsertStart(int data);
	void InsertProcess();
	// 노드 추가 후 불균형 탐색
	void InsertPost();

	// 노드 높이, 균형치 계산 함수
	int Height(AVLNode* node) {
		if (!node) return 0;
		return 1 + (std::max)(Height(node->GetLeft()), Height(node->GetRight()));
	}
	int BalanceFactor(AVLNode* node)
	{
		return Height(node->GetLeft()) - Height(node->GetRight());
	}

	// 불균형 형태 판단 함수
	int DetectCase();
	
	// 자손 노드 교체 함수
	void ReplaceChild(AVLNode* parent, AVLNode* oldChild, AVLNode* newChild);

	// 노드 이동 함수
	void MoveNode(AVLNode* targetNode, Vector2 position);

	// 노드 회전 함수
	void RotateRight(AVLNode* a);
	void RotateLeft(AVLNode* a);

	// 노드 이동 여부 확인 함수 / 하나라도 이동 중 이라면 false
	bool IsAllNodeStop();
	bool IsAllNodeStopRecursive(AVLNode* node);

	// 노드 삭제 함수
	void DeleteStart(int data);
	void DeleteProcess();

	// 모든 노드 삭제 함수
	void DeleteAll();
	void DeleteAllRecursive(AVLNode* node);

	// 모든 노드 깜빡임 함수
	void BlinkAll(AVLNode* root);

	// 노드 검색 함수
	AVLNode* FindNode(int key);

private:
	// 일시 정지 플래그
	bool pause = false;

	// 단계별 작동을 위한 타이머
	Timer timer;

	// AVL 트리의 root
	AVLNode* root = nullptr;

	// 트리의 노드의 개수, 최대 노드 개수 = 31
	int nodeCount = 0;

	// root 의 위치
	Vector2 rootPosition = Vector2(280, 15);

	// 입력중 플래그
	bool isInputting = false;

	// 입력 중인 숫자
	int inputValue = 0;

	// 노드 삽입 or 삭제
	bool mode = false;

	// 삽입 중인 노드 포인터
	AVLNode* insertingNode = nullptr;

	// 삽입 시 비교 중인 노드 포인터
	AVLNode* comparingNode = nullptr;

	// 삽입 함수 콜 횟수 (노드의 위치를 정하기 위한 변수)
	int locationLevel = 0;

	// 노드 삽입 시 생성되는 위치
	Vector2 spawnSpot = Vector2(280, 30);

	// 노드 삽입 중임을 알리는 플래그
	int isInserting = 0;

	// 노드 회전 단계 진행 변수
	int rotateLevel = 0;

	// 모든 노드 삭제 플래그
	int allClear = false;

	// 노드 회전 단계에서 사용할 임시 포인터
	AVLNode* b = nullptr;
	AVLNode* c = nullptr;

	// 삽입, 삭제 상태에서 불균형노드
	AVLNode* z = nullptr;
	AVLNode* y = nullptr;
	AVLNode* x = nullptr;

	// 삭제 상태 단계 변수
	int isDeleting = 0;

	// 삭제할 노드 포인터
	AVLNode* deletingNode = nullptr;

	// 숫자 출력을 위한 문자열
	const std::string bigNumber[10][5] =
	{
		{"#####", "#   #", "#   #", "#   #", "#####"},
		{"#", "#", "#", "#", "#"},
		{"#####", "    #", "#####", "#    ", "#####"},
		{"#####", "    #", "#####", "    #", "#####"},
		{"#   #", "#   #", "#####", "    #", "    #"},
		{"#####", "#    ", "#####", "    #", "#####"},
		{"#####", "#    ", "#####", "#   #", "#####"},
		{"#####", "    #",  "    #","    #", "    #"},
		{"#####", "#   #", "#####" ,"#   #", "#####"},
		{"#####", "#   #", "#####", "    #", "    #"}
	};
};