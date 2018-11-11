/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "ClothLevel.cpp"
* Description	: ClothLevel Implementation file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/
#include "ClothLevel.h"



ClothLevel::ClothLevel()
{
	Init();
}


ClothLevel::~ClothLevel()
{
}

void ClothLevel::Init()
{
	auto cloth = std::make_shared<Cloth>();
	cloth->Init(5, 5, 11, 11, glm::vec3(0.0f, 3.0f, 55.0f)); //14, 10, 55, 45

	m_cloth = std::move(cloth);

	//Initialise text
	auto xText = std::make_unique<TextLabel>("x: ", "Resources/Fonts/bubble.TTF", glm::vec2());
	auto yText = std::make_unique<TextLabel>("y: ", "Resources/Fonts/bubble.TTF", glm::vec2(Utils::SCR_WIDTH / 2, 0.0f));
	auto instr1 = std::make_unique<TextLabel>("Reset cloth with 'R'", "Resources/Fonts/arial.TTF", glm::vec2(30.0f, Utils::SCR_HEIGHT - 30.0f));
	auto instr2 = std::make_unique<TextLabel>("Remove pins 'T'", "Resources/Fonts/arial.TTF", glm::vec2(30.0f, Utils::SCR_HEIGHT - 60.0f));
	auto instr3 = std::make_unique<TextLabel>("Randomly delete a constraint 'F'", "Resources/Fonts/arial.TTF", glm::vec2(30.0f, Utils::SCR_HEIGHT - 90.0f));
	auto instr4 = std::make_unique<TextLabel>("Hold Scroll button + UJ (up, down) HK (left, right) to navigate with camera", "Resources/Fonts/arial.TTF", glm::vec2(30.0f, 60.0f));
	auto instr5 = std::make_unique<TextLabel>("Control player via WASD + QE(up, down)", "Resources/Fonts/arial.TTF", glm::vec2(30.0f, 30.0f));
	auto instr6 = std::make_unique<TextLabel>("Toggle Ball movement and win control with 'B'", "Resources/Fonts/arial.TTF", glm::vec2(30.0f, Utils::SCR_HEIGHT - 120.0f));


	auto windLabelX = std::make_unique<TextLabel>("Wind X: ", "Resources/Fonts/arial.TTF", glm::vec2(Utils::SCR_WIDTH - 200.0f, Utils::SCR_HEIGHT - 30.0f));
	auto windLabelY = std::make_unique<TextLabel>("Wind Y: ", "Resources/Fonts/arial.TTF", glm::vec2(Utils::SCR_WIDTH - 200.0f, Utils::SCR_HEIGHT - 60.0f));
	auto windLabelZ = std::make_unique<TextLabel>("Wind Z: ", "Resources/Fonts/arial.TTF", glm::vec2(Utils::SCR_WIDTH - 200.0f, Utils::SCR_HEIGHT - 90.0f));

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
	auto player = std::make_unique<Player>(0.5f);
	player->SetPos(glm::vec3(0.0f, -0.8f, 60.0f));
	m_player = std::move(player);

	auto floor = std::make_unique<Shapes>();
	floor->Init(SPR_BACKGROUND, CUBE);
	floor->SetPos(glm::vec3(0.0f, -10.0f, 50.0f));
	floor->SetScale(glm::vec3(30.0f, 2.0f, 30.0f));
	m_mEntitiesList["Floor"] = std::move(floor);
}

void ClothLevel::Render()
{
	m_player->Render();
	m_cloth->Render();

	Scene::Render(); //Render last as entities and text should go on top
}

/***********************
* Process: Process cloth level
* @author: Vivian Ngo
***********************/
void ClothLevel::Process(float _deltaTick)
{
	//On-screen mouse coordinates
	m_mTextList.find("MouseX")->second->SetText("x: " + std::to_string(Input::GetInstance()->m_fMouseX));
	m_mTextList.find("MouseY")->second->SetText("y: " + std::to_string(Input::GetInstance()->m_fMouseY));

	m_player->Process(_deltaTick);


	Scene::Process(_deltaTick);
	ClothInput(_deltaTick); //Handle cloth
}

void ClothLevel::ClothInput(float _deltaTick)
{
	//Handle world wind that affects the cloth
	if (!m_player->GetMoveable())
	{
		float windStrength = 0.001f;
		if (Input::GetInstance()->KeyState['A'] == INPUT_HOLD ||
			Input::GetInstance()->KeyState['a'] == INPUT_HOLD)
		{
			windX -= windStrength;
		}
		if (Input::GetInstance()->KeyState['D'] == INPUT_HOLD ||
			Input::GetInstance()->KeyState['d'] == INPUT_HOLD)
		{
			windX += windStrength;
		}
		if (Input::GetInstance()->KeyState['W'] == INPUT_HOLD ||
			Input::GetInstance()->KeyState['w'] == INPUT_HOLD)
		{
			windZ -= windStrength;
		}
		else if (Input::GetInstance()->KeyState['S'] == INPUT_HOLD ||
			Input::GetInstance()->KeyState['s'] == INPUT_HOLD)
		{
			windZ += windStrength;
		}
		else if (Input::GetInstance()->KeyState['Q'] == INPUT_HOLD ||
			Input::GetInstance()->KeyState['q'] == INPUT_HOLD)
		{
			windY += windStrength;
		}
		else if (Input::GetInstance()->KeyState['E'] == INPUT_HOLD ||
			Input::GetInstance()->KeyState['e'] == INPUT_HOLD)
		{
			windY -= windStrength;
		}

		//Set wind text
		m_mTextList["WindX"]->SetText("Wind X: " + std::to_string(windX));
		m_mTextList["WindY"]->SetText("Wind Y: " + std::to_string(windY));
		m_mTextList["WindZ"]->SetText("Wind Z: " + std::to_string(windZ));

		//Generate wind
		m_cloth->WindForce(glm::vec3(windX, windY, windZ)*_deltaTick / 100.0f);
	}
	else
	{
		//Turn off wind
		m_mTextList["WindX"]->SetText(" ");
		m_mTextList["WindY"]->SetText(" ");
		m_mTextList["WindZ"]->SetText(" ");
		windX = 0.0f;
		windY = 0.0f;
		windZ = 0.0f;
		m_cloth->WindForce(glm::vec3(0.0f, 0.0f, 0.0f));
	}

	//Add gravity to cloth
	m_cloth->AddForce(glm::vec3(0, -0.000048f, 0)*_deltaTick);
	m_cloth->Process(_deltaTick);

	//Handle ball collision with cloth
	m_cloth->BallCollision(m_player->GetPos(), m_player->GetRadius());

	//Reset cloth scene
	if (Input::GetInstance()->KeyState['r'] == INPUT_FIRST_PRESS ||
		Input::GetInstance()->KeyState['R'] == INPUT_FIRST_PRESS)
	{
		m_cloth->Init(5, 5, 11, 11, glm::vec3(0.0f, 3.0f, 55.0f)); //14, 10, 55, 45
		windX = 0.0f;
		windY = 0.0f;
		windZ = 0.0f;
		m_mTextList["WindX"]->SetText(" ");
		m_mTextList["WindY"]->SetText(" ");
		m_mTextList["WindZ"]->SetText(" ");
		m_cloth->WindForce(glm::vec3(0.0f, 0.0f, 0.0f));
	}

	//Toggle b button to switch wind control on and off
	if (Input::GetInstance()->KeyState['b'] == INPUT_FIRST_PRESS ||
		Input::GetInstance()->KeyState['B'] == INPUT_FIRST_PRESS)
	{
		if (m_player->GetMoveable())
		{
			m_player->SetMoveable(false);
		}
		else
		{
			m_player->SetMoveable(true);
		}
	}

	//Unpin all pinned particles
	if (Input::GetInstance()->KeyState['t'] == INPUT_FIRST_PRESS ||
		Input::GetInstance()->KeyState['T'] == INPUT_FIRST_PRESS)
	{
		m_cloth->UnpinAll();
	}

	//Delete a random constraint off the cloth
	if (Input::GetInstance()->KeyState['f'] == INPUT_FIRST_PRESS ||
		Input::GetInstance()->KeyState['F'] == INPUT_FIRST_PRESS)
	{
		m_cloth->DeleteRandomConstraint();
	}

	//z to squish rings and x to expand ring positions
	if (Input::GetInstance()->KeyState['z'] == INPUT_HOLD ||
		Input::GetInstance()->KeyState['Z'] == INPUT_HOLD)
	{
		m_cloth->MoveRings(true);
	}
	else if (Input::GetInstance()->KeyState['x'] == INPUT_HOLD ||
		Input::GetInstance()->KeyState['X'] == INPUT_HOLD)
	{
		m_cloth->MoveRings(false);
	}
}
