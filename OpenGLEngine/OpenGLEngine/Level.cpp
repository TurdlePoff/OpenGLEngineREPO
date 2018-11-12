/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "Level.cpp"
* Description	: Level Implementation file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/
#include "Level.h"


/***********************
* Level Constructor
* @author: Vivian Ngo
***********************/
Level::Level()
{
	Init();
}


/***********************
* Level Destructor
* @author: Vivian Ngo
***********************/
Level::~Level(){}

void Level::Init()
{
	//Initialise text
	auto xText = std::make_shared<TextLabel>("x: ", "Resources/Fonts/bubble.TTF", glm::vec2());
	auto yText = std::make_shared<TextLabel>("y: ", "Resources/Fonts/bubble.TTF", glm::vec2(Utils::SCR_WIDTH / 2, 0.0f));
	auto instr1 = std::make_shared<TextLabel>("Reset cloth with 'R'", "Resources/Fonts/arial.TTF", glm::vec2(30.0f, Utils::SCR_HEIGHT - 30.0f));
	auto instr2 = std::make_shared<TextLabel>("Remove pins 'T'", "Resources/Fonts/arial.TTF", glm::vec2(30.0f, Utils::SCR_HEIGHT - 60.0f));
	auto instr3 = std::make_shared<TextLabel>("Randomly delete a constraint 'F'", "Resources/Fonts/arial.TTF", glm::vec2(30.0f, Utils::SCR_HEIGHT - 90.0f));
	auto instr4 = std::make_shared<TextLabel>("Hold Scroll button + UJ (forward, backwards) HK (left, right) to navigate with camera", "Resources/Fonts/arial.TTF", glm::vec2(30.0f, 60.0f));
	auto instr5 = std::make_shared<TextLabel>("Control player via WASD", "Resources/Fonts/arial.TTF", glm::vec2(30.0f, 30.0f));
	auto instr6 = std::make_shared<TextLabel>("Toggle Ball movement and win control with 'B'", "Resources/Fonts/arial.TTF", glm::vec2(30.0f, Utils::SCR_HEIGHT - 120.0f));

	auto windLabelX = std::make_shared<TextLabel>("Wind X: ", "Resources/Fonts/arial.TTF", glm::vec2(Utils::SCR_WIDTH - 200.0f, Utils::SCR_HEIGHT - 30.0f));
	auto windLabelY = std::make_shared<TextLabel>("Wind Y: ", "Resources/Fonts/arial.TTF", glm::vec2(Utils::SCR_WIDTH - 200.0f, Utils::SCR_HEIGHT - 60.0f));
	auto windLabelZ = std::make_shared<TextLabel>("Wind Z: ", "Resources/Fonts/arial.TTF", glm::vec2(Utils::SCR_WIDTH - 200.0f, Utils::SCR_HEIGHT - 90.0f));

	m_mTextList["MouseX"] = std::move(xText);
	m_mTextList["MouseY"] = std::move(yText);
	m_mTextList["Instr1"] = std::move(instr1);
	m_mTextList["Instr2"] = std::move(instr2);
	m_mTextList["Instr3"] = std::move(instr3);
	m_mTextList["Instr4"] = std::move(instr4);
	m_mTextList["Instr5"] = std::move(instr5);
	m_mTextList["Instr6"] = std::move(instr6);

	m_mTextList["WindX"] = std::move(windLabelX);
	m_mTextList["WindY"] = std::move(windLabelY);
	m_mTextList["WindZ"] = std::move(windLabelZ);

	//Initialise player object to move
	auto player = std::make_shared<Player>(1.0f);
	player->SetPos(glm::vec3(0.0f, 0.0f, 0.0f));
	m_mEntitiesList["Player"] = std::move(player);

	auto terrain = std::make_shared<Terrain>();
	terrain->InitialiseTerrain();
	m_terrain = std::move(terrain);

	auto ps = std::make_shared<ParticleSystem>(glm::vec3(0.0f, 0.0f, 0.0f), SPR_SNOWFLAKE);
	m_particleSystem = std::move(ps); 

	m_bIsOnGround = true;

	auto model = std::make_shared<Model>("Resources/Sprites/pug/Dog 1.obj");
	m_puggo = std::move(model);


	/*auto cubeMap = std::make_shared<CubeMap>();m_chara
	cubeMap->InitCubeMap();
	m_cubeMap = std::move(cubeMap);*/
}

/***********************
* Render: Render scene objects
* @author: Vivian Ngo
***********************/
void Level::Render()
{

	//m_cubeMap->Render();
	m_terrain->RenderTerrain(); //Render Terrain
	Scene::Render(); //Render last as entities and text should go on top
	m_particleSystem->Render();
	m_puggo->Render();
}

/***********************
* Process: Process scene objects
* @author: Vivian Ngo
* @parameter: _deltaTick - machine time to process
***********************/
void Level::Process(float _deltaTick)
{
	//Turn mesh on and off
	if (Input::GetInstance()->KeyState['m'] == INPUT_FIRST_PRESS || Input::GetInstance()->KeyState['M'] == INPUT_FIRST_PRESS)
	{
		if (m_terrain->GetIsMeshOn())
		{
			m_terrain->SetMeshOn(false);
		}
		else
		{
			m_terrain->SetMeshOn(true);
		}
	}

	//On-screen mouse coordinates
	m_mTextList.find("MouseX")->second->SetText("x: " + std::to_string(Input::GetInstance()->m_fMouseX));
	m_mTextList.find("MouseY")->second->SetText("y: " + std::to_string(Input::GetInstance()->m_fMouseY));

	//Handle player y position when on terrain
	float x = m_mEntitiesList["Player"]->GetPos().x;
	float y = m_terrain->GetHeight(m_mEntitiesList["Player"]->GetPos())
		+ m_mEntitiesList["Player"]->GetScale().y;
	float z = m_mEntitiesList["Player"]->GetPos().z;

	//Checks if player is off of the terrain
	if (!isnan(y) && y != -99998)
	{
		previousHeight = y;
		m_mEntitiesList["Player"]->SetPos(glm::vec3(x, y, z));

	}
	else
	{
		m_mEntitiesList["Player"]->SetPos(glm::vec3(x, previousHeight, z));
	}
	m_particleSystem->Process(_deltaTick);
	

	Scene::Process(_deltaTick);

}