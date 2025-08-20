#pragma once

#include "Level/Level.h"

class BSTSimulatorLevel : public Level
{
	RTTI_DECLARATIONS(BSTSimulatorLevel, Level)

public:
	BSTSimulatorLevel();
	~BSTSimulatorLevel();

	void Tick(float deltaTime) override;
	void Render() override;

private:

};