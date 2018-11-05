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

#include "Utility.h"
#include "Entity.h"

/* Some physics constants */
#define DAMPING 0.01f // how much to damp the cloth simulation each frame
#define TIME_STEPSIZE2 0.5f * 0.5f // how large time step each particle takes each frame



class Particle
{
public:
	Particle() {};
	Particle(glm::vec3 _pos);
	~Particle();

	void Process(float _deltaTick);

	//Particle Functions
	void AddForce(glm::vec3 _f);
	void AddGravity(const glm::vec3 _direction);

	void AdjusttPos(const glm::vec3 v) { if (!m_bIsPinned) m_vPos += v; }
	void SetPinned(bool _isPinned) { m_bIsPinned = _isPinned; }
	void SetPos(glm::vec3 Movement) { m_vPos = Movement; }
	glm::vec3 GetPos() { return m_vPos; }
	glm::vec3 GetOldPos() { return m_vOldPosition; }
	int GetVertID() { return m_iVertID; }
	int GetIndicesID() { return m_iIndicesID; }
	void SetVertID(int _id) { m_iVertID = _id; }
	void SetIndicesID(int _id) { m_iIndicesID = _id; }
	void SetBroken(bool _broken) { m_constraintBroken = _broken; }
	bool GetBroken() { return m_constraintBroken; }

private:
	bool m_bIsPinned;
	float m_fMass;
	int m_iVertID;
	int m_iIndicesID;
	bool m_constraintBroken;

	glm::vec3 m_vPos;
	glm::vec3 m_vOldPosition;
	glm::vec3 m_vAcceleration;

};

