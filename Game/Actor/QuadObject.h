#pragma once

#include "Actor/Actor.h"

class QuadObject : public Actor
{
	RTTI_DECLARATIONS(QuadObject, Actor)

public:
	QuadObject(int id, Color color, Vector2 position);

	void Tick(float deltaTime) override;
	void Render() override;
private:
	int id;
};