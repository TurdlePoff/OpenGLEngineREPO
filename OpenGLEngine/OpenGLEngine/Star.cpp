#pragma once
/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name	: "Star.cpp"
* Description	: Terrain Declaration file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "Star.h"
#include "Camera.h"
#include "ShaderLoader.h"

Star::Star()
{
}


Star::~Star()
{
}

void Star::InitialiseStar()
{
	m_program = ShaderLoader::GetInstance()->GetProgram((char*)"Star");

	GLfloat points[] = {
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, //passing in 1 point
	};

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Star::Render()
{
	glUseProgram(m_program);
	//ModelMatrix
	glm::mat4 translation = glm::translate(glm::mat4(), m_vPos);

	glm::mat4 model = translation;

	glm::mat4 mvp = Camera::GetInstance()->GetProjection() * Camera::GetInstance()->GetView() * model;

	GLint vpLoc = glGetUniformLocation(m_program, "mvp");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glBindVertexArray(m_vao);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}
