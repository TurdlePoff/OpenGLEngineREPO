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
	void Init(const char* _imageType, float _radius = 1.0f);

	virtual void Render();

	void SetRadius(float _radius) { m_radius = _radius; };
	float GetRadius() { return m_radius; };

private:
	float m_radius = 1.0f;
};

