#pragma once
/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "Scene.h"
* Description	: Scene Declaration file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "Utility.h"
#include "TextLabel.h"
#include "Entity.h"
#include "Player.h"
#include "Input.h"
#include "Camera.h"

class Scene
{
public:
	Scene();
	//Scene(int _sceneNum);
	~Scene();

	virtual void Render();
	virtual void Process(float _deltaTick);

protected:
	std::map<std::string, std::shared_ptr<TextLabel>> m_mTextList;
	std::map<std::string, std::shared_ptr<Entity>> m_mEntitiesList;
};