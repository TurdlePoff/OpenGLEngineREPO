#pragma once

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


private:
	int m_iLives; 
	float m_fMovementSpeed = 1.0f;

};

