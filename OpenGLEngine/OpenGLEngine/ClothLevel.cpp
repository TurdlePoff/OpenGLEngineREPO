/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "ClothLevel.cpp"
* Description	: ClothLevel Implementation file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/
#include "ClothLevel.h"



ClothLevel::ClothLevel()
{
}


ClothLevel::~ClothLevel()
{
}

void ClothLevel::Init()
{
}

void ClothLevel::Render()
{

	Scene::Render(); // Render on top
}

/***********************
* Process: Process cloth level
* @author: Vivian Ngo
***********************/
void ClothLevel::Process(float _deltaTick)
{
	Scene::Process(_deltaTick);
}
