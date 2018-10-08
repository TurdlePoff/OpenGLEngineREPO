#include "Player.h"
#include "Input.h"
#include "Terrain.h"


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
	m_fMovementSpeed = 0.5f;
	//std::cout << "CamX: " << m_vPos.x << " CamY: " << m_vPos.y << "CamZ: " << m_vPos.z << std::endl;
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
