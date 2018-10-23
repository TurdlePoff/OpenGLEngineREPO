/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "Cloth.h"
* Description	: Cloth Implementation file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/
#include "Cloth.h"

Cloth::Cloth(float _width, float _height, int _numParticlesWidth, int _numParticlesHeight) : m_fParticlesWidth(_numParticlesWidth), m_fParticlesHeight(_numParticlesHeight)
{
	m_vParticles.resize(_numParticlesWidth * _numParticlesHeight); //I am essentially using this vector as an array with room for _numParticlesWidth*_numParticlesHeight particles

	// creating particles in a grid of particles from (0,0,0) to (width,-height,0)
	for (int x = 0; x<_numParticlesWidth; x++)
	{
		for (int y = 0; y<_numParticlesHeight; y++)
		{
			glm::vec3 pos = glm::vec3(_width * (x / (float)_numParticlesWidth),
				-_height * (y / (float)_numParticlesHeight),
				0);
			m_vParticles[y*_numParticlesWidth + x] = Particle(pos); // insert particle in column x at y'th row
		}
	}

	// Connecting immediate neighbor particles with constraints (distance 1 and sqrt(2) in the grid)
	for (int x = 0; x<_numParticlesWidth; x++)
	{
		for (int y = 0; y<_numParticlesHeight; y++)
		{
			if (x<_numParticlesWidth - 1) MakeConstraint(GetParticle(x, y), GetParticle(x + 1, y));
			if (y<_numParticlesHeight - 1) MakeConstraint(GetParticle(x, y), GetParticle(x, y + 1));
			if (x<_numParticlesWidth - 1 && y<_numParticlesHeight - 1) MakeConstraint(GetParticle(x, y), GetParticle(x + 1, y + 1));
			if (x<_numParticlesWidth - 1 && y<_numParticlesHeight - 1) MakeConstraint(GetParticle(x + 1, y), GetParticle(x, y + 1));
		}
	}

	// Connecting secondary neighbors with constraints (distance 2 and sqrt(4) in the grid)
	for (int x = 0; x<_numParticlesWidth; x++)
	{
		for (int y = 0; y<_numParticlesHeight; y++)
		{
			if (x<_numParticlesWidth - 2) MakeConstraint(GetParticle(x, y), GetParticle(x + 2, y));
			if (y<_numParticlesHeight - 2) MakeConstraint(GetParticle(x, y), GetParticle(x, y + 2));
			if (x<_numParticlesWidth - 2 && y<_numParticlesHeight - 2) MakeConstraint(GetParticle(x, y), GetParticle(x + 2, y + 2));
			if (x<_numParticlesWidth - 2 && y<_numParticlesHeight - 2) MakeConstraint(GetParticle(x + 2, y), GetParticle(x, y + 2));
		}
	}


	// making the upper left most three and right most three particles unmovable
	for (int i = 0; i<3; i++)
	{
		GetParticle(0 + i, 0)->AdjusttPos(glm::vec3(0.5, 0.0, 0.0)); // moving the particle a bit towards the center, to make it hang more natural - because I like it ;)
		GetParticle(0 + i, 0)->SetPinned(true);

		GetParticle(0 + i, 0)->AdjusttPos(glm::vec3(-0.5, 0.0, 0.0)); // moving the particle a bit towards the center, to make it hang more natural - because I like it ;)
		GetParticle(_numParticlesWidth - 1 - i, 0)->SetPinned(true);
	}
}


Cloth::~Cloth()
{
}

void Cloth::Process(float _deltaTick)
{
	std::vector<Constraint>::iterator constraint;
	for (int i = 0; i<CONSTRAINT_ITERATIONS; i++) // iterate over all constraints several times
	{
		for (constraint = m_vConstraints.begin(); constraint != m_vConstraints.end(); constraint++)
		{
			(*constraint).Process(_deltaTick); // satisfy constraint.
		}
	}

	std::vector<Particle>::iterator particle;
	for (particle = m_vParticles.begin(); particle != m_vParticles.end(); particle++)
	{
		(*particle).Process(_deltaTick); // calculate the position of each particle at the next time step.
	}
}

void Cloth::AddForce(const glm::vec3 _direction)
{
	std::vector<Particle>::iterator particle;
	for (particle = m_vParticles.begin(); particle != m_vParticles.end(); particle++)
	{
		(*particle).AddForce(_direction); // add the forces to each particle
	}
}

void Cloth::WindForce(const glm::vec3 _direction)
{
	for (int x = 0; x<m_fParticlesWidth - 1; x++)
	{
		for (int y = 0; y<m_fParticlesHeight - 1; y++)
		{
			AddWindForcesForTriangle(GetParticle(x + 1, y), GetParticle(x, y), GetParticle(x, y + 1), _direction);
			AddWindForcesForTriangle(GetParticle(x + 1, y + 1), GetParticle(x + 1, y), GetParticle(x, y + 1), _direction);
		}
	}
}

void Cloth::BallCollision(const glm::vec3 _center, const float _radius)
{
	std::vector<Particle>::iterator particle;
	for (particle = m_vParticles.begin(); particle != m_vParticles.end(); particle++)
	{
		glm::vec3 v = (*particle).GetPos() - _center;
		float l = v.length();
		if (v.length() < _radius) // if the particle is inside the ball
		{
			(*particle).AdjusttPos(glm::normalize(v) * (_radius - l)); // project the particle to the surface of the ball
		}
	}
}

Particle * Cloth::GetParticle(int x, int y)
{
	return &m_vParticles[y * m_fParticlesWidth + x];
}

void Cloth::MakeConstraint(Particle * p1, Particle * p2)
{
	m_vConstraints.push_back(Constraint(p1, p2));
}

glm::vec3 Cloth::CalcTriangleNormal(Particle * p1, Particle * p2, Particle * p3)
{
	glm::vec3 pos1 = p1->GetPos();
	glm::vec3 pos2 = p2->GetPos();
	glm::vec3 pos3 = p3->GetPos();

	glm::vec3 v1 = pos2 - pos1;
	glm::vec3 v2 = pos3 - pos1;

	return glm::cross(v1, v2);
}

void Cloth::AddWindForcesForTriangle(Particle * p1, Particle * p2, Particle * p3, const glm::vec3 direction)
{
	glm::vec3 normal = CalcTriangleNormal(p1, p2, p3);
	glm::vec3 d = glm::normalize(normal);
	glm::vec3 force = normal * (glm::dot(d, direction));
	p1->AddForce(force);
	p2->AddForce(force);
	p3->AddForce(force);
}
