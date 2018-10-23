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
* Description	: Particle Implementation file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "Particle.h"


Particle::Particle()
{
}

Particle::~Particle()
{
}

void Particle::Init()
{
}

void Particle::Render()
{
}

void Particle::Process(float _deltaTick)
{
	//iterate through all connections and see if destroyed
	//(get and set destroy) if destroyed, delete 
	if (!m_bIsPinned)
	{
		glm::vec3 temp = m_vPos;
		m_vPos = m_vPos + (m_vPos - m_vOldPosition) * (1.0f - DAMPING) + m_vAcceleration * (TIME_STEPSIZE2 * _deltaTick);
		m_vOldPosition = temp;
		m_vAcceleration = glm::vec3(0, 0, 0); // acceleration is reset since it HAS been translated into a change in position (and implicitely into velocity)	
	}
}

void Particle::AddForce(glm::vec3 _f)
{
	m_vAcceleration += _f / m_fMass;
}