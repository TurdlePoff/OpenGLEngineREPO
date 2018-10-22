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
* Player: Player Constructor
* @author: Vivian Ngo
* @date: 04/10/18
***********************/
Player::Player()
{
	Init(SPR_BIRD, CUBE);
}

/***********************
* Render: Render player
* @author: Vivian Ngo
* @date: 04/10/18
***********************/
void Player::Render()
{
	Entity::Render();
}

/***********************
* Process: Process player
* @author: Vivian Ngo
* @date: 04/10/18
* @parameter: _deltaTick - machine time to process
***********************/
void Player::Process(float _deltaTick)
{
	Entity::Process(_deltaTick);
	MovePlayer(_deltaTick);
}

/***********************
* MovePlayer: Move player
* @author: Vivian Ngo
* @date: 04/10/18
* @parameter: _deltaTick - machine time to process
***********************/
void Player::MovePlayer(float _deltaTick)
{
	m_fMovementSpeed = 0.1f * _deltaTick;

	if (Input::GetInstance()->KeyState['W'] == INPUT_HOLD || Input::GetInstance()->KeyState['w'] == INPUT_HOLD)
	{
		m_vPos.z -= m_fMovementSpeed ;
	}
	if (Input::GetInstance()->KeyState['S'] == INPUT_HOLD || Input::GetInstance()->KeyState['s'] == INPUT_HOLD)
	{
		m_vPos.z += m_fMovementSpeed;
	}
	if (Input::GetInstance()->KeyState['A'] == INPUT_HOLD || Input::GetInstance()->KeyState['a'] == INPUT_HOLD)
	{
		m_vPos.x -= m_fMovementSpeed;
	}
	if (Input::GetInstance()->KeyState['D'] == INPUT_HOLD || Input::GetInstance()->KeyState['d'] == INPUT_HOLD)
	{
		m_vPos.x += m_fMovementSpeed;
	}
}
