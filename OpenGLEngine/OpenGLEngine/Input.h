#pragma once

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2018 Media Design School
//
// File Name	: "Input.cpp"
// Description	: Input declaration file
// Author		: Vivian Ngo
// Mail			: vivian.ngo7572@mediadesign.school.nz
//

#include "Utility.h"

class Input
{
public: 
	static Input* GetInstance();
	static void DestroyInstance();
	~Input();

	static void Keyboard_Down(unsigned char keyPressed, int x, int y);
	static void Keyboard_Up(unsigned char keyPressed, int x, int y);
	static void MouseClicked(int buttonPressed, int glutState, int x, int y);
	static void MousePassiveMovement(int x, int y);
	static void MouseScrollHold(int x, int y);
	static void ScollCallback(int button, int glutState, int xOffset, int yOffset);
	static void SpecialKeyPress(int key, int x, int y);
	static void SpecialKeyRelease(int key, int x, int y);

	//Handle glut functions
	static void ProcessInput();

	//Global
	static float m_fMouseX;
	static float m_fMouseY;
	static unsigned int MouseState[3];
	static unsigned int KeyState[255];
	static unsigned int SpecKeyState[4];
	static unsigned int SpaceState[255];

private:
	Input() {};
	static Input* s_pInputInstance;
	static GLfloat MouseSensitivity;
	static GLfloat Yaw;
	static GLfloat Pitch;
	static GLfloat Roll;
	static GLfloat LastX;
	static GLfloat LastY;
	static float LastScrollY;

	static bool FirstMouse;
	

	
};

