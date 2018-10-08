#pragma once
/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name	: "Camera.h"
* Description	: Camera Declaration file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "Utility.h"

class Clock
{
public:
	Clock(Clock const&) = delete;
	void operator=(Clock const&) = delete;
	~Clock() {};

	static Clock* GetInstance();
	static void DestroyInstance();
	static float GetDeltaTick();
	static void Update();

private:
	static Clock* s_pClockInstance;
	Clock();
	
	static std::chrono::high_resolution_clock::time_point m_fOldTime;
	static std::chrono::high_resolution_clock::time_point m_fCurrentTime;
	static float m_fTimeElapsed;
	static float m_fDeltaTick;

};

