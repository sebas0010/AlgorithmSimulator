#pragma once

#include "Utils/Timer.h"
#include "Actor/Actor.h"

class AVLNode : public Actor
{
	RTTI_DECLARATIONS(AVLNode, Actor)
public:
	AVLNode(int data, Vector2 position);

	void Tick(float deltaTime) override;
	void Render() override;

	// 해당 위치로 이동 함수
	void MoveStart(Vector2 targetPosition, float speed);


	// 색깔 바꾸기
	void ChangeColor(Color newColor) { color = newColor; }

	// 부모 자식 노드 Getter/Setter
	AVLNode* GetParent() const { return parent; }
	void SetParent(AVLNode* newParent) { parent = newParent; }

	AVLNode* GetLeft() const { return left; }
	void SetLeft(AVLNode* newLeft) { left = newLeft; }

	AVLNode* GetRight() const { return right; }
	void SetRight(AVLNode* newRight) { right = newRight; }

	int GetData() const { return data; }

	bool IsMoving() const { return isMoving; }
private:
	// 부모
	AVLNode* parent = nullptr;

	// 왼쪽 자손
	AVLNode* left = nullptr;

	// 오른쪽 자손
	AVLNode* right = nullptr;

	// 데이터
	int data;

	// 깜빡이기 위한 플래그
	int blink = 0;

	// 색깔 코드
	int code = 0;

	// 딜레이 효과를 주기 위한 타이머
	Timer timer;

	// 부드럽게 움직이기 위한 위치 float 좌표
	float xLocation = 0.0f;
	float yLocation = 0.0f;

	// 이동 목표 위치
	Vector2 targetPosition;

	// 이동 중 플래그
	bool isMoving = false;

	// 이동 속도
	float moveSpeed = 80;

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

inline bool IsInsideEllipse(int x, int y, Vector2 c, int rx, int ry, float shrink = 0.0f)
{
	// shrink > 0 이면 "조금 더 줄인 원" 내부만 막을 수 있음(테두리 보전용 여유)
	float rxf = rx - shrink;
	float ryf = ry - shrink;
	if (rxf <= 0 || ryf <= 0) return false;

	float nx = (x - c.x) / rxf;
	float ny = (y - c.y) / ryf;
	return (nx * nx + ny * ny) < 1.0f; // 내부면 true
}