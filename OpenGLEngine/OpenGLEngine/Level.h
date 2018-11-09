#pragma once
/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "Level.h"
* Description	: Level Declaration file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "Scene.h"
#include "CubeMap.h"
#include "Terrain.h"
#include "Star.h"
#include "Cloth.h"

class Level : public Scene
{
public:
	Level();
	~Level();

	void Init();
	void Render();
	void Process(float _deltaTick);
	void ClothInput(float _deltaTick);

private: 

	std::shared_ptr<Terrain> m_terrain;

	bool m_bIsOnGround;

	float m_fHeightOffset = 5.0f;
	float previousHeight = 0.0f;
	std::shared_ptr<Cloth> m_cloth;

	float windX = 0.0f;
	float windY = 0.0f;
	float windZ = 0.0f;
};