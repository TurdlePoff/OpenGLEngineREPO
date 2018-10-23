#include "Cloth.h"



Cloth::Cloth()
{
}


Cloth::~Cloth()
{
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

void Cloth::AddWind(Particle * p1, Particle * p2, Particle * p3, const glm::vec3 direction)
{
}
