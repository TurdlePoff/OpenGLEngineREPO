#pragma once
#include "Utility.h"

class Star
{
public:
	Star();
	~Star() {};

	void InitialiseStar();
	virtual void Render();

	//Setters and Getters
	void SetPos(glm::vec3 Movement) { m_vPos = Movement; }
	void SetRotatation(glm::vec3 Rotation) { m_vRot = Rotation; }
	void SetScale(glm::vec3 scale) { m_vScale = scale; }

	glm::vec3 GetPos() { return m_vPos; }
	glm::vec3 GetRot() { return m_vRot; }
	glm::vec3 GetScale() { return m_vScale; }

private:
	unsigned int m_iIndicesCount;
	GLuint m_vbo, m_vao, m_ebo, m_program;

	glm::vec3 m_vPos;	//Sprite position
	glm::vec3 m_vRot;	//Sprite rotation
	glm::vec3 m_vScale;	//Sprite Scale
};

