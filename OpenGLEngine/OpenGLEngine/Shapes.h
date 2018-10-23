#pragma once
/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name	: "Shapes.h"
* Description	: Shapes declaration file. Extends entity 
					and allows user to create different shapes
					Available shapes
						- 2D quad
						- 3D cube
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "Entity.h"
class Shapes : public Entity
{
public:
	Shapes();
	~Shapes();

	void Init(const char* _imageType, EShapes _shape);
	virtual void Render();
};

