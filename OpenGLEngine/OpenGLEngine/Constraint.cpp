/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "Constraint.h"
* Description	: Constraint Implementation file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/


#include "Constraint.h"

Constraint::Constraint(Particle* _p1, Particle* _p2) 
	: m_p1(_p1), m_p2(_p2), m_bDestroyed(false)
{
	glm::vec3 vec = m_p1->GetPos() - m_p2->GetPos();
	m_fRestDist = vec.length();
}


Constraint::~Constraint()
{

}

void Constraint::Render()
{
	//GL_LINES

}

void Constraint::Process(float _deltaTick)
{
	glm::vec3 p1_to_p2 = m_p2->GetPos() - m_p1->GetPos(); // vector from p1 to p2
	float current_distance = p1_to_p2.length(); // current distance between p1 and p2

	if (current_distance < m_fRestDist + 0.5f)
	{
		//Set this to destroy //erase constraint frm vec if destroyed
		m_bDestroyed = true;
		return;
	}

	glm::vec3 correctionVector = p1_to_p2 * (1 - m_fRestDist / current_distance); // The offset vector that could moves p1 into a distance of rest_distance to p2
	glm::vec3 correctionVectorHalf = correctionVector * 0.5f; // Lets make it half that length, so that we can move BOTH p1 and p2.

	m_p1->AdjusttPos(correctionVectorHalf); // correctionVectorHalf is pointing from p1 to p2, so the length should move p1 half the length needed to satisfy the constraint.
	m_p2->AdjusttPos(-correctionVectorHalf); // we must move p2 the negative direction of correctionVectorHalf since it points from p2 to p1, and not p1 to p2.	
}
