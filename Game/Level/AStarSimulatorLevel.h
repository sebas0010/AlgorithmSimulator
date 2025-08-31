#pragma once

#include "Level/Level.h"
#include "Math/Vector2.h"
#include "Actor/AStarNode.h"

#include <queue>
#include <unordered_map>
#include <unordered_set>

class AStarPlayer;
class AStarSimulatorLevel : public Level
{
	RTTI_DECLARATIONS(AStarSimulatorLevel, Level)

public:
	AStarSimulatorLevel();
	~AStarSimulatorLevel();

	void Tick(float deltaTime) override;
	void Render() override;

	// 목표 지점 생성
	void SetGoal(Vector2 targetPosition);
	// 벽 생성
	void SetWall(Vector2 targetPosition);
	// 시작 지점 생성
	void SetStart(Vector2 targetPosition);
	// 해당하는 위치에 위치한 오브젝트 제거
	void DeleteObject(Vector2 targetPosition);
	// 모두 제거
	void AllClear();

	// 알고리즘 시작
	void Simulator();

	void StartAStar();
	void StepAStar();

	// 경로 도착
	void Finish(AStarNode* node);
	// 경로 불가능
	void Finish();

	// 화면 좌표 Getter
	int GetMinX() const { return minX; }
	int GetMinY() const { return minY; }
	int GetMaxX() const { return maxX; }
	int GetMaxY() const { return maxY; }
private:
	bool pause = false;

	AStarPlayer* player = nullptr;

	Actor* goal = nullptr;
	Actor* start = nullptr;

	int minX = 1;
	int maxX = 100;
	int minY = 3;
	int maxY = 32;



	// 시뮬레이터 진행 중임을 알리는 플래그
	int inProcess = 0;


	// 노드 값 비교 연산
	struct CompareNodePtr {
		Vector2 goal;

		CompareNodePtr() : goal(Vector2::Zero) {}          // 기본 생성자
		CompareNodePtr(Vector2 g) : goal(g) {}

		bool operator()(AStarNode* a, AStarNode* b) const {
			return a->F(goal) > b->F(goal); // f가 작은 게 먼저
		}
	};

	// 이동 후보 노드
	std::priority_queue<AStarNode*, std::vector<AStarNode*>, CompareNodePtr> open;

	struct VecHash {
		size_t operator()(const Vector2& p) const noexcept {
			return (static_cast<size_t>(p.x) << 32) ^ static_cast<size_t>(p.y);
		}
	};
	struct VecEq {
		bool operator()(const Vector2& a, const Vector2& b) const noexcept {
			return a.x == b.x && a.y == b.y;
		}
	};

	std::unordered_map<Vector2, int, VecHash, VecEq> gScore;
	std::unordered_set<Vector2, VecHash, VecEq> closed;
};