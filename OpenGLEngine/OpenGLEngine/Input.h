#pragma once
/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "Input.cpp"
* Description	: Input Declaration file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "Utility.h"

class Input
{
public: 
	static Input* GetInstance();
	static void DestroyInstance();
	~Input();

	void Update();
	glm::vec3 ScreenToWorldRay();

	void Keyboard_Down(unsigned char keyPressed, int x, int y);
	void Keyboard_Up(unsigned char keyPressed, int x, int y);
	void MouseClicked(int buttonPressed, int glutState, int x, int y);
	void MousePassiveMovement(int x, int y);
	void MouseClickHold(int x, int y);
	void ScrollCallback(int button, int glutState, int xOffset, int yOffset);
	void SpecialKeyPress(int key, int x, int y);
	void SpecialKeyRelease(int key, int x, int y);

	//Handle glut functions
	void HandleInput();

	//Global
	float m_fMouseX;
	float m_fMouseY;
	unsigned int MouseState[3];
	unsigned int KeyState[255];
	unsigned int SpecKeyState[4];

private:
	Input() {};
	static Input* s_pInputInstance;
	GLfloat MouseSensitivity = 0.2f;
	GLfloat Yaw = 0.0f;
	GLfloat Pitch = 0.0f;
	GLfloat Roll = 0.0f;
	GLfloat LastX = (float)Utils::SCR_WIDTH;
	GLfloat LastY = (float)Utils::SCR_HEIGHT;
	GLfloat xPassiveOffset = 0;
	GLfloat yPassiveOffset = 0;
	GLfloat LastScrollY = 365.0f;
	bool FirstMouse = true;

	//Mouse picking
	glm::vec3 rayDirection;
	float m_fMousePickingX = 0.0f;
	float m_fMousePickingY = 0.0f;
};

