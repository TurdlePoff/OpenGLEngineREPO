/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "Clock.cpp"
* Description	: Clock Implementation file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/
#include "Clock.h"

Clock* Clock::s_pClockInstance = nullptr;

Clock::Clock()
{
	m_fCurrentTime = std::chrono::high_resolution_clock::now();
}

/***********************
* GetInstance: Gets the instance of the clock singleton class
* @author: Vivian Ngo
* @return: s_pClockInstance - Instance of the clock singleton class
***********************/
Clock* Clock::GetInstance()
{
	if (s_pClockInstance == 0)
	{
		s_pClockInstance = new Clock();
	}
	return s_pClockInstance;
}

/***********************
* DestroyInstance: Destroys the instance of the clock singleton class if there is one
* @author: Vivian Ngo
***********************/
void Clock::DestroyInstance()
{
	if (s_pClockInstance != 0) // If there is an instance of this class
	{
		//Delete the instance
		delete s_pClockInstance;
		s_pClockInstance = nullptr;
	}
}

/***********************
* GetDeltaTime: Gets the delta time of the program
* @author: Vivian Ngo
* @return: delta time of the program
***********************/
float Clock::GetDeltaTick()
{
	return m_fDeltaTick;
}

/***********************
* Update: Updates the clock
* @author: Vivian Ngo
***********************/
void Clock::Update()
{
	m_fOldTime = m_fCurrentTime;

	m_fCurrentTime = std::chrono::high_resolution_clock::now();

	m_fDeltaTick = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>
										(m_fCurrentTime - m_fOldTime).count());

	m_fTimeElapsed += m_fDeltaTick;
}

