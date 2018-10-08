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
#include "Camera.h"

/***********************
* Scene: Constructor
* @author: Vivian Ngo
* @date: 04/10/18
***********************/
Scene::Scene(int _sceneNum)
	: m_sceneNumber(_sceneNum)
{
	InitScene();
	m_bIsOnGround = true;
}

/***********************
* InitScene: Initialises scene objects
* @author: Vivian Ngo
* @date: 04/10/18
***********************/
void Scene::InitScene()
{
	//Initialise text
	auto xText = std::make_unique<TextLabel>("x: ", "Resources/Fonts/bubble.TTF", glm::vec2());
	auto yText = std::make_unique<TextLabel>("y: ", "Resources/Fonts/bubble.TTF", glm::vec2(Utils::SCR_WIDTH / 2, 0.0f));
	auto instr = std::make_unique<TextLabel>("Turn on Mesh by pressing 'n' and 'm' to turn it off", "Resources/Fonts/arial.TTF", glm::vec2(100.0f, Utils::SCR_HEIGHT - 50.0f));

	m_mTextList["MouseX"] = std::move(xText);
	m_mTextList["MouseY"] = std::move(yText);
	m_mTextList["Instr1"] = std::move(instr);

	//Initialise terrain
	auto ter = std::make_unique<Terrain>();
	ter->InitialiseTerrain();
	m_terrain = std::move(ter);

	//Initialise Star
	auto star = std::make_unique<Star>();
	star->InitialiseStar();
	m_star = std::move(star);

	//Initialise player object to move
	auto player = std::make_unique<Player>();
	player->SetPos(glm::vec3(0.0f, 0.0f, 50.0f));
	m_mEntitiesList["Player"] = std::move(player);
}

/***********************
* Render: Render scene objects
* @author: Vivian Ngo
* @date: 04/10/18
***********************/
void Scene::Render()
{
	m_terrain->RenderTerrain(); //Render Terrain
	m_star->Render();			//Render Star

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

	//Turn mesh on or off
	if (Input::KeyState['n'] == INPUT_HOLD || Input::KeyState['N'] == INPUT_HOLD)
	{
		m_terrain->SetMeshOn(true);
	}
	if (Input::KeyState['m'] == INPUT_HOLD || Input::KeyState['M'] == INPUT_HOLD)
	{
		m_terrain->SetMeshOn(false);
	}

	//On-screen mouse coordinates
	m_mTextList.find("MouseX")->second->SetText("x: " + std::to_string(Input::m_fMouseX));
	m_mTextList.find("MouseY")->second->SetText("y: " + std::to_string(Input::m_fMouseY));

	//Handle player y position when on terrain
	float x = m_mEntitiesList["Player"]->GetPos().x;
	float y = m_terrain->GetHeight(m_mEntitiesList["Player"]->GetPos())
		+ m_mEntitiesList["Player"]->GetScale().y;
	float z = m_mEntitiesList["Player"]->GetPos().z;

	//Checks if player is off of the terrain
	if (!isnan(y) && y != -99998)
	{
		yPrevious = y;
		m_mEntitiesList["Player"]->SetPos(glm::vec3(x, y, z));

	}
	else
	{
		m_mEntitiesList["Player"]->SetPos(glm::vec3(x, yPrevious, z));

	}

	//Set the star position above the player
	float xStarPos =  m_mEntitiesList["Player"]->GetPos().x;
	float yStarPos = m_mEntitiesList["Player"]->GetPos().y + 5.0f;
	float zStarPos = m_mEntitiesList["Player"]->GetPos().z;

	m_star->SetPos(glm::vec3(xStarPos, yStarPos, zStarPos));
}