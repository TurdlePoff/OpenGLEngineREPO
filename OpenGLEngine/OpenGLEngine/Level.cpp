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
	cloth->Init(10, 10, 10, 10); //14, 10, 55, 45
	cloth->SetPos(glm::vec3(0.0f, 5.0f, 60.0f));

	m_cloth = std::move(cloth);

	//Initialise text
	auto xText = std::make_unique<TextLabel>("x: ", "Resources/Fonts/bubble.TTF", glm::vec2());
	auto yText = std::make_unique<TextLabel>("y: ", "Resources/Fonts/bubble.TTF", glm::vec2(Utils::SCR_WIDTH / 2, 0.0f));
	//auto instr = std::make_unique<TextLabel>("Toggle Terrain Wireframe with 'M'", "Resources/Fonts/arial.TTF", glm::vec2(100.0f, Utils::SCR_HEIGHT - 50.0f));

	m_mTextList["MouseX"] = std::move(xText);
	m_mTextList["MouseY"] = std::move(yText);
	//m_mTextList["Instr1"] = std::move(instr);

	//Initialise player object to move
	auto player = std::make_unique<Player>(1.0f);
	player->SetPos(glm::vec3(0.0f, 0.0f, 50.0f));
	m_player = std::move(player);

	auto floor = std::make_unique<Shapes>();
	floor->Init(SPR_BACKGROUND, CUBE);
	floor->SetPos(glm::vec3(0.0f, -10.0f, 50.0f));
	floor->SetScale(glm::vec3(30.0f, 2.0f, 30.0f));
	m_mEntitiesList["Floor"] = std::move(floor);

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
	m_player->Render();
	m_cloth->Render();
	//m_terrain->RenderTerrain(); //Render Terrain

	Scene::Render(); //Render last as entities and text should go on top
}

/***********************
* Process: Process scene objects
* @author: Vivian Ngo
* @parameter: _deltaTick - machine time to process
***********************/
void Level::Process(float _deltaTick)
{
	//On-screen mouse coordinates
	m_mTextList.find("MouseX")->second->SetText("x: " + std::to_string(Input::GetInstance()->m_fMouseX));
	m_mTextList.find("MouseY")->second->SetText("y: " + std::to_string(Input::GetInstance()->m_fMouseY));
	m_player->Process(_deltaTick);

	m_cloth->AddForce(glm::vec3(0, -0.0000098f, 0)*_deltaTick);


	if (Input::GetInstance()->KeyState['a'])
	{
		windHorizontal = 1.0f;
	}
	else if(Input::GetInstance()->KeyState['d'])
	{
		windHorizontal = 1.0f;
	}
	else if (Input::GetInstance()->KeyState['w'])
	{
		windVertical = 1.0f;
	}
	else if (Input::GetInstance()->KeyState['s'])
	{
		windVertical = 1.0f;
	}

	m_cloth->WindForce(glm::vec3(windHorizontal, windVertical, -0.2)*_deltaTick/100000.0f); // generate some wind each frame
	m_cloth->Process(_deltaTick);
	m_cloth->BallCollision(m_player->GetPos(), m_player->GetRadius()); // resolve collision with the ball

	Scene::Process(_deltaTick);
}