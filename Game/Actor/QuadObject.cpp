#include "QuadObject.h"

QuadObject::QuadObject(int id, Color color, Vector2 position)
	: Actor(std::to_string(id).c_str(), color, position), id(id)
{
	
}

void QuadObject::Tick(float deltaTime)
{
	super::Tick(deltaTime);
}

void QuadObject::Render()
{
	super::Render();
}
