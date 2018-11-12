/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "Player.cpp"
* Description	: Player Implementation file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/
#include "Player.h"
#include "Input.h"
#include "Terrain.h"

/***********************
* Player Constructor
* @author: Vivian Ngo
***********************/
Player::Player(float _radius)
{
	Init(SPR_STRIPE, _radius);
}
//
/***********************
* Render player
* @author: Vivian Ngo
***********************/
void Player::Render()
{
	Shapes::Render();
}

//void Player::Render(Terrain * _terrain)
//{
//	m_chara->render(.016f, _terrain);
//
//}

/***********************
* Process: Process player
* @author: Vivian Ngo
* @parameter: _deltaTick - machine time to process
***********************/
void Player::Process(float _deltaTick)
{
	Shapes::Process(_deltaTick);
	if (m_bPlayerMoveable)
	{
		MovePlayer(_deltaTick);
	}
}

/***********************
* MovePlayer: Move player
* @author: Vivian Ngo
* @parameter: _deltaTick - machine time to process
***********************/
void Player::MovePlayer(float _deltaTick)
{
	m_fMovementSpeed = 0.04f * _deltaTick;

	if (Input::GetInstance()->KeyState['W'] == INPUT_HOLD || 
		Input::GetInstance()->KeyState['w'] == INPUT_HOLD)
	{
		m_vPos.z -= m_fMovementSpeed ;
	}
	if (Input::GetInstance()->KeyState['S'] == INPUT_HOLD || 
		Input::GetInstance()->KeyState['s'] == INPUT_HOLD)
	{
		m_vPos.z += m_fMovementSpeed;
	}
	if (Input::GetInstance()->KeyState['A'] == INPUT_HOLD || 
		Input::GetInstance()->KeyState['a'] == INPUT_HOLD)
	{
		m_vPos.x -= m_fMovementSpeed;
	}
	if (Input::GetInstance()->KeyState['D'] == INPUT_HOLD || 
		Input::GetInstance()->KeyState['d'] == INPUT_HOLD)
	{
		m_vPos.x += m_fMovementSpeed;
	}
	/*
	//Turn off up and down as player should be stuck to the terrain
	if (Input::GetInstance()->KeyState['Q'] == INPUT_HOLD || 
		Input::GetInstance()->KeyState['q'] == INPUT_HOLD)
	{
		m_vPos.y += m_fMovementSpeed;
	}
	if (Input::GetInstance()->KeyState['E'] == INPUT_HOLD || 
		Input::GetInstance()->KeyState['e'] == INPUT_HOLD)
	{
		m_vPos.y -= m_fMovementSpeed;
	}*/
}
