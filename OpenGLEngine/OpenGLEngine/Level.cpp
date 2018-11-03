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
	auto cloth = std::make_shared<Cloth>();
	cloth->Init(5, 5, 10, 10);
	cloth->SetPos(glm::vec3(0.0f, 10.0f, 50.0f));

	m_cloth = std::move(cloth);

	//Initialise text
	auto xText = std::make_unique<TextLabel>("x: ", "Resources/Fonts/bubble.TTF", glm::vec2());
	auto yText = std::make_unique<TextLabel>("y: ", "Resources/Fonts/bubble.TTF", glm::vec2(Utils::SCR_WIDTH / 2, 0.0f));
	auto instr = std::make_unique<TextLabel>("Toggle Terrain Wireframe with 'M'", "Resources/Fonts/arial.TTF", glm::vec2(100.0f, Utils::SCR_HEIGHT - 50.0f));

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
	//m_star = std::move(star);
	m_mEntitiesList["Star"] = std::move(star);

	//Initialise player object to move
	auto player = std::make_unique<Player>();
	player->SetPos(glm::vec3(0.0f, 0.0f, 50.0f));
	m_mEntitiesList["Player"] = std::move(player);

	//Initialise player object to move
	auto m_Sphere = std::make_unique<Shapes>();
	m_Sphere->Init(SPR_STRIPE, 10.0f);

	m_Sphere->SetPos(glm::vec3(-20.0f, 10.0f, 50.0f));
	m_mEntitiesList["Sphere"] = std::move(m_Sphere);

	m_bIsOnGround = true;

	/*auto cubeMap = std::make_unique<CubeMap>();
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

	m_cloth->Render();
	m_terrain->RenderTerrain(); //Render Terrain

	Scene::Render(); //Render last as entities and text should go on top
}

/***********************
* Process: Process scene objects
* @author: Vivian Ngo
* @parameter: _deltaTick - machine time to process
***********************/
void Level::Process(float _deltaTick)
{
	//Turn mesh on and off
	if (Input::GetInstance()->KeyState['m'] == INPUT_FIRST_PRESS ||		Input::GetInstance()->KeyState['M'] == INPUT_FIRST_PRESS)
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

	//Set the star position above the player
	float xStarPos = m_mEntitiesList["Player"]->GetPos().x;
	float yStarPos = m_mEntitiesList["Player"]->GetPos().y + 5.0f;
	float zStarPos = m_mEntitiesList["Player"]->GetPos().z;

	m_mEntitiesList["Star"]->SetPos(glm::vec3(xStarPos, yStarPos, zStarPos));


	m_cloth->Process(_deltaTick);

	Scene::Process(_deltaTick);
}