#pragma once

#include "Level/Level.h"

class AStarSimulatorLevel : public Level
{
	RTTI_DECLARATIONS(AStarSimulatorLevel, Level)

public:
	AStarSimulatorLevel();
	~AStarSimulatorLevel();

	void Tick(float deltaTime) override;
	void Render() override;

private:
	
	int leftX = 1;
	int rightX = 100;
	int upY = 3;
	int downY = 32;
};