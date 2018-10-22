/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "Scene.cpp"
* Description	: Scene Implementation file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "Scene.h"

/***********************
* Scene: Constructor
* @author: Vivian Ngo
* @date: 04/10/18
***********************/
Scene::Scene()
{
}

Scene::~Scene()
{
	m_mTextList.clear();
	m_mEntitiesList.clear();
}

/***********************
* Render: Render scene objects
* @author: Vivian Ngo
* @date: 04/10/18
***********************/
void Scene::Render()
{
	//Render all entities
	for (auto it = m_mEntitiesList.begin(); it != m_mEntitiesList.end(); ++it)
	{
		it->second->Render();
	}

	//Render all text in text list 
	for (auto it = m_mTextList.begin(); it != m_mTextList.end(); ++it)
	{
		it->second->Render();
	}
}

/***********************
* Process: Process scene objects
* @author: Vivian Ngo
* @date: 04/10/18
* @parameter: _deltaTick - machine time to process
***********************/
void Scene::Process(float _deltaTick)
{
	//Process all entities in scene
	for (auto it = m_mEntitiesList.begin(); it != m_mEntitiesList.end(); ++it)
	{
		it->second->Process(_deltaTick);
	}
}