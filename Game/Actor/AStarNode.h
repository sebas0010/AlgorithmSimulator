#pragma once

#include "Actor/Actor.h"

class AStarNode : public Actor
{
    RTTI_DECLARATIONS(AStarNode, Actor)

public:
    AStarNode(Vector2 newPosition, int g, Vector2 goalPosition)
        : Actor("@", Color::Intensity, newPosition), g(g), goalPosition(goalPosition)
    {
    }

    // g / f 값
    int GetG() const { return g; }
    void SetG(int newG) { g = newG; }

    // h 계산 (맨해튼 거리)
    int Heuristic(const Vector2& goalPosition) const {
        return abs(position.x - goalPosition.x) + abs(position.y - goalPosition.y);
    }

    int F(const Vector2& goalPosition) const {
        return g + Heuristic(goalPosition);
    }

    // 경로 추적용
    void SetParent(AStarNode* p) { parent = p; }
    AStarNode* GetParent() const { return parent; }

    // 방문 여부
    bool visited = false;

    // 색깔 바꾸기
    void ChangeColor(Color newColor) { color = newColor; }

private:
    // 현재 까지 cost
    int g;
    // 목표 지점 위치
    Vector2 goalPosition;

    AStarNode* parent = nullptr; // 경로 재구성용
};
