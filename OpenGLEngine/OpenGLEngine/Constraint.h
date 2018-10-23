#pragma once
/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "Constraint.h"
* Description	: Constraint Declaration file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "Utility.h"
#include "Particle.h"

class Constraint
{
public:
	Constraint(Particle* _p1, Particle* _p2);
	~Constraint();

	void Render();
	void Process(float _deltaTick);
	void Destroy() { m_bDestroyed = true; };
	bool GetDestroyed() { return m_bDestroyed; };

private:
	Particle *m_p1, *m_p2;
	float m_fRestDist;
	bool m_bDestroyed;

};

