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

	float windHorizontal = 0.0f;
	float windVertical = 0.0f;
private: 

	//std::unique_ptr<CubeMap> m_cubeMap;
	//std::unique_ptr<Terrain> m_terrain;
	//std::unique_ptr<Shapes> m_Sphere;
	std::unique_ptr<Shapes> m_player;

	bool m_bIsOnGround;
	float m_fHeightOffset = 5.0f;
	float previousHeight = 0.0f;
	std::shared_ptr<Cloth> m_cloth;

};