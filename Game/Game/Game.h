#pragma once

#include "Engine.h"
#include "Level/Level.h"

class Game : public Engine
{
public:

	Game();

private:
	Level* backLevel = nullptr;
};