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

#define CONSTRAINT_ITERATIONS 5

#include "Utility.h"
#include "Particle.h"
#include "Constraint.h"
#include "Entity.h"

class Cloth : public Entity
{
public:
	Cloth();

	~Cloth();

	void Init(float _width, float _height, int _numParticlesWidth, int _numParticlesHeight, glm::vec3 _pos);
	void GenerateBuffers();
	void Render();
	void Process(float _deltaTick);
	void AddForce(const glm::vec3 _direction);
	void WindForce(const glm::vec3 _direction);
	void BallCollision(const glm::vec3 _center, const float _radius);
	void UnpinAll();
	void DeleteRandomConstraint();
	void ProcessParticlePick(Particle* particle);
	void MoveRings(bool _squish);

private:
	std::vector<float> m_fVerticesPoints;
	std::vector<int> m_iIndicesPoints;

	float m_fWidth;
	float m_fHeight;

	int m_fParticlesWidth; // number of particles in "width" direction
	int m_fParticlesHeight; // number of particles in "height" direction

	bool m_isHoldingParticle = false;
	Particle* pickedParticle;

	std::vector<Particle> m_vParticles; // all particles that are part of this cloth
	std::vector<Constraint> m_vConstraints; // alle constraints between particles as part of this cloth

	Particle* GetParticle(int x, int y);
	void MakeConstraint(Particle *p1, Particle *p2);

	glm::vec3 CalcTriangleNormal(Particle *p1, Particle *p2, Particle *p3);
	void AddWindForcesForTriangle(Particle *p1, Particle *p2, Particle *p3, const glm::vec3 direction);
};

