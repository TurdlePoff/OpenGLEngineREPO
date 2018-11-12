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
Particle::Particle(glm::vec3 _origin, glm::vec3 _vec, float _elapsedTime, float _maxSpeed, int _id) :
	m_vPos(_origin), m_vOrigin(_origin), m_vOriginalVelocity(_vec),  m_vVelocity(_vec), m_fElapsedTime(_elapsedTime), m_fMaxSpeed(_maxSpeed), m_iParticleID(_id)
{
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
	m_vVelocity.y += -0.0001f * _deltaTick;
	m_vPos += m_vVelocity;
	m_fCameraDistance = glm::distance(Camera::GetInstance()->GetCamPos(), m_vPos);

	if (m_vPos.y <= -100.0f)
	{
		m_vPos = m_vOrigin;
		m_vVelocity = m_vOriginalVelocity;
	}
}