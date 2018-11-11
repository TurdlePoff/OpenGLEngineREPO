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
#include "ClothParticle.h"

class Constraint
{
public:
	Constraint() {};

	Constraint(ClothParticle* _p1, ClothParticle* _p2);
	~Constraint();

	void Render();
	void Process(float _deltaTick);
	void CheckConstraintLength(glm::vec3 _length, float _currentDistance);

	void Destroy() { m_bDestroyed = true; };
	bool GetDestroyed() { return m_bDestroyed; };
	ClothParticle* GetParticleOne() { return m_p1; };
	ClothParticle* GetParticleTwo() { return m_p2; };
	float GetRestDist() {
		return m_fRestDist;	}

private:
	ClothParticle *m_p1, *m_p2;
	float m_fRestDist;
	bool m_bDestroyed;
	float m_fSensitivity;

	glm::vec3 p1_to_p2;
};

