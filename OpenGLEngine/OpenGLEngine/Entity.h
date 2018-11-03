#pragma once
/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name	: "Entity.h"
* Description	: Entity declaration file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "Utility.h"
#include "ShaderLoader.h"
#include "Camera.h"

class Entity
{
public:
	Entity();
	~Entity();

	virtual void Process(float _deltaTick);
	virtual void Render();

	//Setters and Getters
	void SetPos(glm::vec3 Movement) { m_vPos = Movement; }
	void SetRotatation(glm::vec3 Rotation) { m_vRot = Rotation; }
	void SetScale(glm::vec3 scale) { m_vScale = scale; }

	glm::vec3 GetPos() { return m_vPos; }
	glm::vec3 GetRot() { return m_vRot; }
	glm::vec3 GetScale() { return m_vScale; }

protected:
	unsigned int m_iIndicesCount, m_iVerticesCount;
	GLuint m_vbo, m_vao, m_ebo, m_texture, m_program;
	int m_iWidth, m_iHeight;

	glm::vec3 m_vPos;	//Sprite position
	glm::vec3 m_vRot;	//Sprite rotation
	glm::vec3 m_vScale;	//Sprite Scale
	glm::vec4 m_vColour;//Sprite Colour
	EShapes m_eShape;
	EImage m_eImageType;
};

