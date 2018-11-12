#pragma once
/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "Particle.h"
* Description	: Particle Declaration file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "Entity.h"

class Particle
{
public:
	Particle() {};
	Particle(glm::vec3 _pos, glm::vec3 _vec, float _elapsedTime, float _maxSpeed, int _id);
	~Particle();

	void Render() {};
	void Process(float _deltaTick);

	glm::vec3 GetPos() { return m_vPos;	}
	float GetCamDist() { return m_fCameraDistance; }

private:
	glm::vec3 m_vVelocity = glm::vec3();
	glm::vec3 m_vOriginalVelocity = glm::vec3();

	glm::vec3 m_vOrigin = glm::vec3();
	glm::vec3 m_vPos = glm::vec3();

	float m_fCameraDistance = 0.0f;
	float m_fElapsedTime = 0.0f;
	float m_fMaxSpeed = 1.0f;

	int m_iParticleID = 0;

};

