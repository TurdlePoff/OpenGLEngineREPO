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

	if (Input::KeyState['W'] == INPUT_HOLD || Input::KeyState['w'] == INPUT_HOLD)
	{
		m_vPos.z -= m_fMovementSpeed ;
	}
	if (Input::KeyState['S'] == INPUT_HOLD || Input::KeyState['s'] == INPUT_HOLD)
	{
		m_vPos.z += m_fMovementSpeed;
	}
	if (Input::KeyState['A'] == INPUT_HOLD || Input::KeyState['a'] == INPUT_HOLD)
	{
		m_vPos.x -= m_fMovementSpeed;
	}
	if (Input::KeyState['D'] == INPUT_HOLD || Input::KeyState['d'] == INPUT_HOLD)
	{
		m_vPos.x += m_fMovementSpeed;
	}
}
