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
	cloth->Init(5, 5, 10, 10, glm::vec3(0.0f, 3.0f, 55.0f)); //14, 10, 55, 45

	m_cloth = std::move(cloth);

	//Initialise text
	auto xText = std::make_unique<TextLabel>("x: ", "Resources/Fonts/bubble.TTF", glm::vec2());
	auto yText = std::make_unique<TextLabel>("y: ", "Resources/Fonts/bubble.TTF", glm::vec2(Utils::SCR_WIDTH / 2, 0.0f));
	auto instr1 = std::make_unique<TextLabel>("Reset cloth with 'R'", "Resources/Fonts/arial.TTF", glm::vec2(30.0f, Utils::SCR_HEIGHT - 30.0f));
	auto instr2 = std::make_unique<TextLabel>("Remove pins 'T'", "Resources/Fonts/arial.TTF", glm::vec2(30.0f, Utils::SCR_HEIGHT - 60.0f));
	auto instr3 = std::make_unique<TextLabel>("Randomly delete a constraint 'F'", "Resources/Fonts/arial.TTF", glm::vec2(30.0f, Utils::SCR_HEIGHT - 90.0f));
	auto instr4 = std::make_unique<TextLabel>("Hold Scroll button + UJ (up, down) HK (left, right) to navigate with camera", "Resources/Fonts/arial.TTF", glm::vec2(30.0f, 60.0f));
	auto instr5 = std::make_unique<TextLabel>("Control player via WASD + QE(up, down)", "Resources/Fonts/arial.TTF", glm::vec2(30.0f, 30.0f));

	m_mTextList["MouseX"] = std::move(xText);
	m_mTextList["MouseY"] = std::move(yText);
	m_mTextList["Instr1"] = std::move(instr1);
	m_mTextList["Instr2"] = std::move(instr2);
	m_mTextList["Instr3"] = std::move(instr3);
	m_mTextList["Instr4"] = std::move(instr4);
	m_mTextList["Instr5"] = std::move(instr5);

	//Initialise player object to move
	auto player = std::make_unique<Player>(0.5f);
	player->SetPos(glm::vec3(0.0f, -0.8f, 60.0f));
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


	float windDir = 0.1f;
	if (Input::GetInstance()->KeyState['a'] == INPUT_HOLD)
	{
		windHorizontal = -windDir;
	}
	else if(Input::GetInstance()->KeyState['d'] == INPUT_HOLD)
	{
		windHorizontal = windDir;
	}
	else if (Input::GetInstance()->KeyState['w'] == INPUT_HOLD)
	{
		windVertical = windDir;
	}
	else if (Input::GetInstance()->KeyState['s'] == INPUT_HOLD)
	{
		windVertical = windDir;
	}
	
	if (Input::GetInstance()->KeyState['r'] == INPUT_FIRST_PRESS || Input::GetInstance()->KeyState['R'] == INPUT_FIRST_PRESS)
	{
		m_cloth->Init(5, 5, 10, 10, glm::vec3(0.0f, 3.0f, 55.0f)); //14, 10, 55, 45
	}

	if (Input::GetInstance()->KeyState['t'] == INPUT_FIRST_PRESS || Input::GetInstance()->KeyState['T'] == INPUT_FIRST_PRESS)
	{
		m_cloth->UnpinAll();
	}




	m_cloth->AddForce(glm::vec3(0, -0.0000048f, 0)*_deltaTick);

	//m_cloth->WindForce(glm::vec3(windHorizontal, windVertical, -0.01)*_deltaTick/10000.0f); // generate some wind each frame
	m_cloth->Process(_deltaTick);
	m_cloth->BallCollision(m_player->GetPos(), m_player->GetRadius()); // resolve collision with the ball

	Scene::Process(_deltaTick);

	if (Input::GetInstance()->KeyState['f'] == INPUT_FIRST_PRESS || Input::GetInstance()->KeyState['F'] == INPUT_FIRST_PRESS)
	{
		m_cloth->DeleteRandomParticle();
	}

}