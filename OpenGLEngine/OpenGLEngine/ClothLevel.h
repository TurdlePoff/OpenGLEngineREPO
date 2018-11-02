#pragma once
/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "ClothLevel.h"
* Description	: ClothLevel Declaration file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "Scene.h"


class ClothLevel : public Scene
{
public:
	ClothLevel();
	~ClothLevel();

	void Init();
	void Render();
	void Process(float _deltaTick);

};

