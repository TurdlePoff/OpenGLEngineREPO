#pragma once

class Player
{
public:
	Player();
	~Player();

	void SetLives(int newLives) { m_iLives = newLives; }
	int GetLives() { return m_iLives; }


private:
	int m_iLives; 
};

