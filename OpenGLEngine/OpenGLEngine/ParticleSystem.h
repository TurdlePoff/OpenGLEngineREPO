#pragma once
/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "ParticleSystem.h"
* Description	: ParticleSystem Declaration file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "Utility.h"
#include "Particle.h"


class ParticleSystem : public Entity
{
public:
	ParticleSystem() {};
	ParticleSystem(glm::vec3 _origin, const char* _textureName);
	~ParticleSystem();


	struct {
		bool operator()(Particle a, Particle b) const
		{
			return a.GetCamDist() > b.GetCamDist();
		}
	} myComparison;

	void Render();
	void Process(float _deltaTick);
	//bool myComparison(Particle a, Particle b);

	std::vector<Particle> m_vecParticles;
	std::vector<glm::vec3> m_vecPosition;

private:
	int m_iParticles = 4000;
};

