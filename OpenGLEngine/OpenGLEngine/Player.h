#pragma once
/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "Player.h"
* Description	: Player Declaration file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "Shapes.h"

class Player : public Shapes
{
public:
	Player() {};
	Player(float _radius);

	~Player() {};

	void Render();
	void Process(float _deltaTick);
	void MovePlayer(float _deltaTick);

	void SetLives(int newLives) { m_iLives = newLives; }
	int GetLives() { return m_iLives; }
	void SetMoveable(bool _move) {	m_bPlayerMoveable = _move;	}
	bool GetMoveable() { return m_bPlayerMoveable; }

private:
	int m_iLives = 1;
	float m_fMovementSpeed = 1.0f;
	bool m_bPlayerMoveable = true;;

};

