#pragma once

#include "Level/Level.h"
#include "Math/Vector2.h"
#include "Actor/QuadNode.h"

class QuadTreeSimulatorLevel : public Level
{
	RTTI_DECLARATIONS(QuadTreeSimulatorLevel, Level)

public:
	QuadTreeSimulatorLevel();
	~QuadTreeSimulatorLevel();

	void Tick(float deltaTime) override;
	void Render() override;

	// 삽입 함수
	void insert();
	// 삽입 완료 표시
	void insertFinish(int id, Vector2 position);

	// 화면 좌표 Getter
	int GetMinX() const { return minX; }
	int GetMinY() const { return minY; }
	int GetMaxX() const { return maxX; }
	int GetMaxY() const { return maxY; }

private:
	// 일시 정지 플래그
	bool pause = false;

	// 삽입할 오브젝트의 숫자
	int objectId = 1;

	// 삽입중임을 표시하는 플래그
	bool isInserting = false;

	// 화면 좌표
	int minX = 3;
	int minY = 3;
	int maxX = 98;
	int maxY = 34;

	// 쿼드 트리 노드
	QuadNode* root = nullptr;
};