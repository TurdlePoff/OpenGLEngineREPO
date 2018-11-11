#pragma once
/*********************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name	: "Utility.cpp"
* Description	: Utility file for the project
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
**********************************************************/

#ifndef __UTILITY_H__
#define __UTILITY_H__

#define _USE_MATH_DEFINES

#pragma region Sprites
#define SPR_BIRD "Resources/Sprites/noot.png"
#define SPR_TRIBIRD "Resources/Sprites/triNoot.png"
#define SPR_PIG "Resources/Sprites/pig.png"

#define SPR_FLOOR "Resources/Sprites/floorBg.jpg"
#define SPR_BACKGROUND "Resources/Sprites/ocean.jpg"

#define SPR_STRIPE "Resources/Sprites/stripe.jpg"
#define SPR_RED "Resources/Sprites/red.jpg"

#pragma endregion Sprite Locations

//Dependencies
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"
#include "Dependencies\FMOD\fmod.hpp"
//#include <glm/gtx/compatibility.hpp>

//#include  <Box2D/Box2D.h>

//Includes
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <fstream>
#include <filesystem>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdexcept>
#include <assert.h>
#include <chrono>
#include <cmath> 
#include <math.h>

enum EMouse
{
	LCLICK, MCLICK, RCLICK
};

enum InputState {
	INPUT_FIRST_RELEASE, // First frame of Up state 
	INPUT_RELEASED, // Default State (Up) 
	INPUT_FIRST_PRESS, // First frame of Down state 
	INPUT_HOLD, // Key is held Down
};

enum EImage {
	ROUNDBIRB,
	FASTBIRB,
	BODEBIRB,
	PIG,
	FLOOR,
	BACKGROUND,
	NONE
};

enum EShapes {
	QUAD,
	CUBE
};

namespace Utils
{
	static const unsigned int SCR_WIDTH = 1000;
	static const unsigned int SCR_HEIGHT = 600;

	static const float DENSITY = 0.3f;
	static const float FRICTION = 1.0f;

	/***********************
	* RandomFloat: Generates a random float
	* @author: Vivian Ngo
	* @return: r - a random float
	***********************/
	static float RandomFloat()
	{
		float r = (float)rand() / (float)RAND_MAX;
		return r;
	}
}

static const GLuint indicesSquare[] = {
	0, 1, 2, // First Triangle
	0, 2, 3 // Second Triangle
};

static const GLfloat verticesSquare[] = {
	// Positions			// Colors				// Tex Coords
	-0.5f, 0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 0.0f, // Top Left
	0.5f, 0.5f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,		1.0f, 0.0f, // Top Right
	0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 0.0f, 1.0f,		1.0f, 1.0f, // Bottom Right
	-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f, // Bottom Left
};

static const GLuint indicesCube[] = {
	// Front Face
	0, 1, 2,
	0, 2, 3,
	// Right Face
	4, 5, 6,
	4, 6, 7,
	// Back Face
	8, 9, 10,
	8, 10, 11,
	// Left Face
	12, 13, 14,
	12, 14, 15,
	// Top Face	

	16, 17, 18,
	16, 18, 19,
	// Bottom Face
	20, 21, 22,
	20, 22, 23,
};


static GLfloat verticesCube[216] = {
	// Positions // Colors // TexCoords
	-1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f, 0.0f,		0.0f, 0.0f,
	1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	1.0f, -1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 0.0f,		1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,		1.0f, 1.0f, 1.0f, 0.0f,		0.0f, 1.0f,
	// Right Face
	1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	1.0f, 1.0f, -1.0f,		0.0f, 1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	1.0f, -1.0f, -1.0f,		0.0f, 0.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 0.0f,		0.0f, 1.0f,

	// Back Face
	1.0f, 1.0f, -1.0f,		0.0f, 1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	-1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	-1.0f, -1.0f, -1.0f,	0.0f, 0.0f, 1.0f, 0.0f,		1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,		0.0f, 0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
	// Left Face
	-1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	-1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f, 0.0f,		1.0f, 0.0f,
	-1.0f, -1.0f, 1.0f,		1.0f, 0.0f, 1.0f, 0.0f,		1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,	0.0f, 0.0f, 1.0f, 0.0f,		0.0f, 1.0f,

	// Top Face
	-1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f, 0.0f,		0.0f, 0.0f,
	1.0f, 1.0f, -1.0f,		0.0f, 1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f, 0.0f,		0.0f, 1.0f,
	// Bottom Face
	-1.0f, -1.0f, 1.0f,		1.0f, 0.0f, 1.0f, 0.0f,		0.0f, 0.0f,
	1.0f, -1.0f, 1.0f,		0.0f, 1.0f, 1.0f, 0.0f,		1.0f, 0.0f,
	1.0f, -1.0f, -1.0f,		0.0f, 0.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,	0.0f, 0.0f, 1.0f, 0.0f,		0.0f, 1.0f,
};





#endif // !__UTILITY_H__
