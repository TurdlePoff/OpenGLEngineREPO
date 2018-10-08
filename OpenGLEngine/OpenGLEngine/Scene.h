#pragma once
/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name	: "Scene.h"
* Description	: Scene Declaration file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "Utility.h"
#include "TextLabel.h"
#include "Entity.h"
#include "CubeMap.h"
#include "Terrain.h"
#include "Player.h"

class Scene
{
public:
	Scene() {};
	Scene(int _sceneNum);
	~Scene();

	void InitScene();
	void InitStaticObjects();

	void Render();
	void Process(float _deltaTick);

	int GetLevelNum() { return m_sceneNumber; }

private:
	int m_sceneNumber;

	std::map<std::string, std::unique_ptr<TextLabel>> m_mTextList;
	std::map<std::string, std::unique_ptr<Entity>> m_mEntitiesList;

	//std::unique_ptr<b2World> m_world;
	//std::unique_ptr<CubeMap> m_cubeMap;

	std::unique_ptr<Terrain> m_terrain;

};

