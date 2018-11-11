#pragma once
/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name	: "SceneManager.h"
* Description	: SceneManager Declaration file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "SceneManager.h"
#include "Scene.h"
#include "Level.h"
#include "ClothLevel.h"

#include "Utility.h"

class SceneManager
{
public:
	~SceneManager() {};
	static SceneManager* GetInstance();
	static void DestroyInstance();

	void InitScenes();
	//void AddScene(std::shared_ptr<Scene> scene);
	void RemoveScene(std::string _level);
	void SwitchScene(std::string _level);
	void ResetLevels();
	void ProcessScene(float _deltaTick);
	void RenderScene();
	void SetWinner(bool winner) { m_bWinner = winner; }
	bool GetWinner() { return m_bWinner; }
	void SetPlayerPos(glm::vec3 pos) { pPos = pos; }
	glm::vec3 GetPlayerPosition() { return pPos; }

	std::string GetCurrentScene();
	//int GetCurrentSceneNumber();

private:
	static SceneManager* s_pSceneInstance;
	SceneManager() {};

	std::map<std::string, std::shared_ptr<Scene>>::iterator it;
	//std::vector<std::shared_ptr<Scene>> scenesMap;
	/*std::shared_ptr<Scene> currentScene;*/
	std::string m_sCurrentLevel;
	std::map<std::string, std::shared_ptr<Scene>> scenesMap;


	glm::vec3 pPos;
	bool m_bWinner;
	bool m_bScissorOn = true;
	//bool m_bPolygonMode = false;

};

