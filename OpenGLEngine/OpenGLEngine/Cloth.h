#pragma once
/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "Cloth.h"
* Description	: Cloth Declaration file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#define CONSTRAINT_ITERATIONS 15

#include "Utility.h"
#include "Particle.h"
#include "Constraint.h"

class Cloth
{
public:
	Cloth() {};

	Cloth(float width, float height, int num_particles_width, int num_particles_height);
	~Cloth();

	void Process(float _deltaTick);
	void AddForce(const glm::vec3 _direction);
	void WindForce(const glm::vec3 _direction);
	void BallCollision(const glm::vec3 _center, const float _radius);

private:
	int m_fParticlesWidth; // number of particles in "width" direction
	int m_fParticlesHeight; // number of particles in "height" direction


	std::vector<Particle> m_vParticles; // all particles that are part of this cloth
	std::vector<Constraint> m_vConstraints; // alle constraints between particles as part of this cloth

	Particle* GetParticle(int x, int y);
	void MakeConstraint(Particle *p1, Particle *p2);

	glm::vec3 CalcTriangleNormal(Particle *p1, Particle *p2, Particle *p3);
	void AddWindForcesForTriangle(Particle *p1, Particle *p2, Particle *p3, const glm::vec3 direction);

};

