#include "Player.h"
#include "Input.h"



Player::Player()
{
	Init(SPR_BIRD, CUBE);
}


Player::~Player()
{
}

void Player::Render()
{
	Entity::Render();
}

void Player::Process(float _deltaTick)
{
	Entity::Process(_deltaTick);
	MovePlayer(_deltaTick);
}

void Player::MovePlayer(float _deltaTick)
{
	if (Input::KeyState['W'] == INPUT_HOLD || Input::KeyState['w'] == INPUT_HOLD)
	{
		m_vPos.z -= m_fMovementSpeed ;
	}
	else if (Input::KeyState['S'] == INPUT_HOLD || Input::KeyState['s'] == INPUT_HOLD)
	{
		m_vPos.z += m_fMovementSpeed;
	}
	else if (Input::KeyState['A'] == INPUT_HOLD || Input::KeyState['a'] == INPUT_HOLD)
	{
		m_vPos.x -= m_fMovementSpeed;
	}
	else if (Input::KeyState['D'] == INPUT_HOLD || Input::KeyState['d'] == INPUT_HOLD)
	{
		m_vPos.x += m_fMovementSpeed;
	}
}
