#pragma once
#include "Utility.h"
#include "Entity.h"

class Star : public Entity
{
public:
	Star();
	~Star() {};

	void InitialiseStar();
	virtual void Render();
};

