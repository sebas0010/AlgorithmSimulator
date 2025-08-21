#pragma once

#include "Level/Level.h"
#include "Actor/NodeActor.h"

class AVLTreeSimulator : public Level
{
	RTTI_DECLARATIONS(AVLTreeSimulator, Level)

public:
	AVLTreeSimulator();
	~AVLTreeSimulator();

	void Tick(float deltaTime) override;
	void Render() override;

	void GetInput();
private:

	// AVL Æ®¸®ÀÇ »Ñ¸®
	NodeActor* TreeRoot = nullptr;
};