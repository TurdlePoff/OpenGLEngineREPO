/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "Particle.cpp"
* Description	: Particle Implementation file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "Particle.h"
#include "Camera.h"


/***********************
* Particle Constructor
* @author: Vivian Ngo
***********************/
Particle::Particle(glm::vec3 _pos, glm::vec3 _vec, float _elapsedTime, int _id) :
	m_vVelocity(_vec), m_fElapsedTime(_elapsedTime), m_iParticleID(_id)
{
	m_vPos = _pos;
}

/***********************
* Particle Destructor
* @author: Vivian Ngo
***********************/
Particle::~Particle(){}

/***********************
* Process: Process Particle
* @author: Vivian Ngo
* @parameter: _deltaTick - machine time to process
***********************/
void Particle::Process(float _deltaTick)
{
	m_vVelocity.y += -0.2f * 0.0167f;
	m_vPos += m_vVelocity;
	m_fElapsedTime -= 0.000167f;
	m_fCameraDistance = glm::distance(Camera::GetInstance()->GetCamPos(), m_vPos);
	if (m_fElapsedTime <= 0.0f) 
	{
		m_vPos = m_vOrigin;
		m_vVelocity = glm::vec3(0.25f * cos(m_iParticleID * 0.0167f) + 0.25f * Utils::RandomFloat() - 0.125f, 
				1.5f + 0.25f * Utils::RandomFloat() - 0.125f,
				0.25f * sin(m_iParticleID * 0.0167f) + 0.25f * Utils::RandomFloat() - 0.125f);
		m_fElapsedTime = Utils::RandomFloat() + 0.125f;
	}
}