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
	auto scene = std::make_shared<Scene>(1);

	AddScene(scene);

	SwitchScene(1); //Start with start screen
}

/***********************
* AddScene: Add scene to scene manager
* @author: Vivian Ngo
* @date: 04 / 10 / 18
* @parameter: scene - scene to add
***********************/
void SceneManager::AddScene(std::shared_ptr<Scene> scene)
{
	scenesList.push_back(scene);
}

/***********************
* RemoveScene: Remove scene from scene manager
* @author: Vivian Ngo
* @date: 04 / 10 / 18
* @parameter: scene - scene to add
***********************/
void SceneManager::RemoveScene(int scene)
{
	std::shared_ptr<Scene> tempScene;
	for (unsigned int i = 0; i < scenesList.size(); ++i)
	{
		if (scene == scenesList[i]->GetLevelNum())
		{
			tempScene = scenesList[i];
			break;
		}
	}

	it = scenesList.begin();

	it = std::find(scenesList.begin(), scenesList.end(), tempScene);
	if (it != scenesList.end())
	{
		scenesList.erase(it);
		scenesList.shrink_to_fit();
	}
}

/***********************
* SwitchScene: Switch scene to the number specified
* @author: Vivian Ngo
* @date: 04 / 10 / 18
* @parameter: level - scene to change to
***********************/
void SceneManager::SwitchScene(int level)
{
	for (unsigned int i = 0; i < scenesList.size(); ++i)
	{
		if (level == scenesList[i]->GetLevelNum())
		{
			currentScene = scenesList[i];
			break;
		}
	}
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
	currentScene->Process(_deltaTick);
}

/***********************
* RenderScene: Render the scene specified
* @author: Vivian Ngo
* @date: 04 / 10 / 18
* @parameter: scene - scene to render
***********************/
void SceneManager::RenderScene()
{
	currentScene->Render();
}

/***********************
* GetCurrentScene: Gets the current scene
* @author: Vivian Ngo
* @date: 04 / 10 / 18
* @return: currentScene - the current scene
***********************/
std::shared_ptr<Scene> SceneManager::GetCurrentScene()
{
	return currentScene;
}

/***********************
* GetCurrentSceneNumber: Gets the current scene number
* @author: Vivian Ngo
* @date: 04 / 10 / 18
* @return: currentScene->GetLevelNum() - the current scene's level number
***********************/
int SceneManager::GetCurrentSceneNumber()
{
	return currentScene->GetLevelNum();
}
