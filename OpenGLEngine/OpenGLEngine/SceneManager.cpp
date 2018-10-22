/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name	: "SceneManager.cpp"
* Description	: SceneManager implementation file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "SceneManager.h"

SceneManager* SceneManager::s_pSceneInstance = 0;

/***********************
* GetInstance: Gets scene manager Instance
* @author: Vivian Ngo
* @date: 04 / 10 / 18
* return s_pSoundInstance - instance of sound manager
***********************/
SceneManager* SceneManager::GetInstance()
{
	if (s_pSceneInstance == 0)
	{
		s_pSceneInstance = new SceneManager();
	}
	return s_pSceneInstance;
}

/***********************
* DestroyInstance: Destroys scene manager Instance
* @author: Vivian Ngo
* @date: 04 / 10 / 18
***********************/
void SceneManager::DestroyInstance()
{
	if (s_pSceneInstance != 0) // If there is an instance of this class
	{
		//Delete the instance
		delete s_pSceneInstance;
		s_pSceneInstance = nullptr;
	}
}

/***********************
* SetUpScenes: Set up scene items
* @author: Vivian Ngo
* @date: 04 / 10 / 18
***********************/
void SceneManager::InitScenes()
{
	auto terrainLevel = std::make_shared<Level>();
	scenesMap["TerrainLevel"] = (std::move(terrainLevel));
	SwitchScene("TerrainLevel"); //Start with start screen
}

/***********************
* RemoveScene: Remove scene from scene manager
* @author: Vivian Ngo
* @date: 04 / 10 / 18
* @parameter: scene - scene to add
***********************/
void SceneManager::RemoveScene(std::string _level)
{
	it = scenesMap.find(_level);
	scenesMap.erase(scenesMap.find(_level), scenesMap.end());

	/*std::shared_ptr<Scene> tempScene;
	for (unsigned int i = 0; i < scenesMap.size(); ++i)
	{
		if (scene == scenesMap[i]->GetLevelNum())
		{
			tempScene = scenesMap[i];
			break;
		}
	}

	it = scenesMap.begin();

	it = std::find(scenesMap.begin(), scenesMap.end(), tempScene);
	if (it != scenesMap.end())
	{
		scenesMap.erase(it);
		scenesMap.shrink_to_fit();
	}*/
}

/***********************
* SwitchScene: Switch scene to the number specified
* @author: Vivian Ngo
* @date: 04 / 10 / 18
* @parameter: level - scene to change to
***********************/
void SceneManager::SwitchScene(std::string _level)
{
	m_sCurrentLevel = _level;
	/*for (unsigned int i = 0; i < scenesMap.size(); ++i)
	{
		if (level == scenesMap[i]->GetLevelNum())
		{
			currentScene = scenesMap[i];
			break;
		}
	}*/
}

/***********************
* ResetLevels: Method resetting all levels of the game + player stats
* @author: Vivian Ngo
* @date: 04 / 10 / 18
***********************/
void SceneManager::ResetLevels()
{
	/*RemoveScene(1);
	RemoveScene(2);
	RemoveScene(3);

	_player->ResetPlayerStats();
	m_bWinner = false;

	std::shared_ptr<Scene> level1(new CLevel(1, BG, _player));
	std::shared_ptr<Scene> level2(new CLevel(2, BG, _player));
	std::shared_ptr<Scene> level3(new CLevel(3, BG, _player));

	AddScene(level1);
	AddScene(level2);
	AddScene(level3);*/
}

/***********************
* UpdateScene: Update the scene specified
* @author: Vivian Ngo
* @date: 04 / 10 / 18
* @parameter: scene - scene to update
***********************/
void SceneManager::ProcessScene(float _deltaTick)
{
	scenesMap[m_sCurrentLevel]->Process(_deltaTick);
}

/***********************
* RenderScene: Render the scene specified
* @author: Vivian Ngo
* @date: 04 / 10 / 18
* @parameter: scene - scene to render
***********************/
void SceneManager::RenderScene()
{
	scenesMap[m_sCurrentLevel]->Render();
}

/***********************
* GetCurrentScene: Gets the current scene
* @author: Vivian Ngo
* @date: 04 / 10 / 18
* @return: currentScene - the current scene
***********************/
std::string SceneManager::GetCurrentScene()
{
	return m_sCurrentLevel;
}
//
///***********************
//* GetCurrentSceneNumber: Gets the current scene number
//* @author: Vivian Ngo
//* @date: 04 / 10 / 18
//* @return: currentScene->GetLevelNum() - the current scene's level number
//***********************/
//int SceneManager::GetCurrentSceneNumber()
//{
//	return currentScene->GetLevelNum();
//}
