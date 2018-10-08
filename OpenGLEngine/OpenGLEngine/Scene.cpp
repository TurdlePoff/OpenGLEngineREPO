/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name	: "Scene.cpp"
* Description	: Scene Implementation file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "Scene.h"
#include "Input.h"

Scene::Scene(int _sceneNum)
	: m_sceneNumber(_sceneNum)
{
	InitScene();
	InitStaticObjects();
}

Scene::~Scene(){}

void Scene::InitScene()
{
	auto xText = std::make_unique<TextLabel>("x: ", "Resources/Fonts/bubble.TTF", glm::vec2());
	auto yText = std::make_unique<TextLabel>("y: ", "Resources/Fonts/bubble.TTF", glm::vec2(Utils::SCR_WIDTH / 2, 0.0f));

	m_mTextList["MouseX"] = std::move(xText);
	m_mTextList["MouseY"] = std::move(yText);
	m_terrain = std::move(std::make_unique<Terrain>(30, 2));
	//m_terrain->InitialiseTerrain();
	/*m_cubeMap = std::make_unique<CubeMap>();
	m_cubeMap->InitCubeMap();*/
}

void Scene::InitStaticObjects()
{
	auto ground = std::make_unique<Entity>();
	ground->Init(SPR_FLOOR, CUBE);
	ground->SetPos(glm::vec3(-10.0f, -10.0f, -1.0f));
	ground->SetRotatation(glm::vec3(0.0f, 0.0f, -0.0f));
	ground->SetScale(glm::vec3(5, 4.0f, 5.0f));

	m_mEntitiesList["Ground"] = std::move(ground);

}

void Scene::Render()
{
	m_terrain->Render();

	//m_cubeMap->Render();
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

void Scene::Process(float _deltaTick)
{

	m_mTextList.find("MouseX")->second->SetText("x: " + std::to_string(Input::m_fMouseX));
	m_mTextList.find("MouseY")->second->SetText("y: " + std::to_string(Input::m_fMouseY));

}