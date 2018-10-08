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

class Entity
{
public:
	Entity();
	~Entity();

	void Init(const char* _imageType, EShapes _shape);
	//const char* GetTextureFile(EImage _spriteType);
	virtual void Process(float _deltaTick);
	virtual void Render();


	//Setters and Getters
	void SetPos(glm::vec3 Movement);
	void SetRotatation(glm::vec3 Rotation);
	void SetScale(glm::vec3 scale);

	glm::vec3 GetPos();
	glm::vec3 GetRot();
	glm::vec3 GetScale();

	//b2Body* GetBody() { return m_b2body;}
	//void InitB2Body(b2World& _world, b2BodyType _bodyType, EShapes _shape, bool m_rotatable, bool _hasFixture);


protected:
	unsigned int m_iIndicesCount;
	GLuint m_vbo, m_vao, m_ebo, m_texture, m_program;
	int m_iWidth, m_iHeight;

	glm::vec3 m_vPos;	//Sprite position
	glm::vec3 m_vRot;	//Sprite rotation
	glm::vec3 m_vScale;	//Sprite Scale
	glm::vec4 m_vColour;//Sprite Colour
	EShapes m_eShape;
	EImage m_eImageType;

	//b2Body* m_b2body;
};

