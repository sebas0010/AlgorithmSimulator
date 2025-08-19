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

};