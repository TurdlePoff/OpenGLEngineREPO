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
#include "Cloth.h"


class ClothLevel : public Scene
{
public:
	ClothLevel();
	~ClothLevel();

	void Init();
	void Render();
	void Process(float _deltaTick);
	void ClothInput(float _deltaTick);

private:

	std::unique_ptr<Player> m_player;

	bool m_bIsOnGround;

	float m_fHeightOffset = 5.0f;
	float previousHeight = 0.0f;
	std::shared_ptr<Cloth> m_cloth;

	float windX = 0.0f;
	float windY = 0.0f;
	float windZ = 0.0f;
};

