#pragma once

#include "Level/Level.h"

class QuadTreeSimulatorLevel : public Level
{
	RTTI_DECLARATIONS(QuadTreeSimulatorLevel, Level)

public:
	QuadTreeSimulatorLevel();
	~QuadTreeSimulatorLevel();

	void Tick(float deltaTime) override;
	void Render() override;

private:

};