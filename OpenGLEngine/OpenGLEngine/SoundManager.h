#pragma once
/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name	: "SoundManager.h"
* Description	: SoundManager declaration file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

//#include "Utility.h"
#include "Dependencies\FMOD\fmod.hpp"

class CSoundManager
{
public:
	static CSoundManager* GetInstance();
	static void DestroyInstance();

	~CSoundManager() {}; 

	bool InitFmod();
	const bool LoadAudio();
	void InitBackground();
	void InitThump();
	void InitPowNom();

private: 
	static CSoundManager* s_pSoundInstance;
	CSoundManager() {};

	FMOD::System* audioMgr; 
	FMOD::Sound* fxPew; 
	FMOD::Sound* fxPowerUp;

	FMOD::Sound* bgmTheme;
};
