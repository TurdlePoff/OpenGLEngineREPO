/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name	: "SoundManager.cpp"
* Description	: SoundManager implementation file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "SoundManager.h"

CSoundManager* CSoundManager::s_pSoundInstance = 0;

/***********************
* GetInstance: Gets Sound manager Instance
* @author: Vivian Ngo
* return s_pSoundInstance - instance of sound manager
***********************/
CSoundManager* CSoundManager::GetInstance()
{
	if (s_pSoundInstance == 0)
	{
		s_pSoundInstance = new CSoundManager();
	}
	return s_pSoundInstance;
}

/***********************
* DestroyInstance: Destroys Sound manager Instance
* @author: Vivian Ngo
***********************/
void CSoundManager::DestroyInstance()
{
	if (s_pSoundInstance != 0) // If there is an instance of this class
	{
		//Delete the instance
		delete s_pSoundInstance;
		s_pSoundInstance = nullptr;
	}
}

/***********************
* InitFmod: Initialises FMOD
* @author: Vivian Ngo
* @return bool - true if initialised, else, false.
***********************/
bool CSoundManager::InitFmod() 
{
	FMOD_RESULT result; result = FMOD::System_Create(&audioMgr); 

	if (result != FMOD_OK) 
	{ 
		return false; 
	}

	result = audioMgr->init(50, FMOD_INIT_NORMAL, 0); 

	if (result != FMOD_OK) 
	{ 
		return false; 
	} 
	return true;
}

/***********************
* LoadAudio: Loads audio
* @author: Vivian Ngo
* @return const bool - true if loaded successfully
***********************/
const bool CSoundManager::LoadAudio() 
{
	FMOD_RESULT result;

	result = audioMgr->createSound("Resources/sounds/poweruppleNom.wav", FMOD_DEFAULT, 0, &fxPowerUp);
	result = audioMgr->createSound("Resources/sounds/shootBub.wav", FMOD_DEFAULT, 0, &fxPew);
	result = audioMgr->createSound("Resources/sounds/Background.mp3", FMOD_DEFAULT, 0, &bgmTheme);

	fxPew->setMode(FMOD_LOOP_OFF);

	bgmTheme->setMode(FMOD_LOOP_NORMAL);

	return true;
}

/***********************
* InitSound: Plays sound
* @author: Vivian Ngo
***********************/
void CSoundManager::InitBackground()
{
	FMOD::Channel* channel; 
	audioMgr->playSound(bgmTheme, 0, false, &channel);
	channel->setVolume(0.5f);
}

/***********************
* InitSound: Plays sound
* @author: Vivian Ngo
***********************/
void CSoundManager::InitThump()
{
	FMOD::Channel* channel;
	audioMgr->playSound(fxPew, 0, false, &channel);
	channel->setVolume(0.5f);
}

void CSoundManager::InitPowNom()
{
	FMOD::Channel* channel;
	audioMgr->playSound(fxPowerUp, 0, false, &channel);
	channel->setVolume(0.5f);
}
