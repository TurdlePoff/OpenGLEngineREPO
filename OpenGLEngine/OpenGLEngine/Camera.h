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

class Camera
{
public:
	static Camera* GetInstance();
	static void DestroyInstance();

	//static glm::mat4 GetCam();
	void Process(float _deltaTick);
	void CameraMovement(float _deltaTick);

	static void SetFOV(float _fov);
	static float GetFOV();
	static void SetCamFront(glm::vec3 _cFront);

	void SetCamPos(glm::vec3 _cPos);
	void SetCamUp(glm::vec3 _cUp);
	void SetCamSpeed(float _cSpeed);

	glm::vec3 GetCamFront();
	static glm::vec3 GetCamPos();
	glm::vec3 GetCamUp();
	float GetCamSpeed();
	glm::mat4 GetMVP() { return MVP; }
	glm::mat4 GetModel() { return Model; }
	glm::mat4 GetProjection() { return projection; }
	glm::mat4 GetView();
	void SetRotation(glm::vec3 rot) { cameraRot = rot; }
	glm::mat4 GetRotation();

	static void SetCamTarget(glm::vec3 _targetPos) { cameraTarget = _targetPos; }
	static glm::vec3 GetCamTarget() { return cameraTarget; }

private:
	static Camera* s_pCameraInstance;
	Camera();

	static glm::vec3 cameraFront;
	static float m_fov;

	static glm::mat4 view;
	static glm::mat4 projection;

	static glm::vec3 cameraPos;
	static glm::vec3 cameraUp;
	static glm::vec3 cameraTarget;
	static glm::vec3 cameraRot;

	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget); //pointing in reverse of where it is pointing
	glm::vec3 cameraRight = glm::normalize(glm::cross(cameraUp, cameraDirection));

	float cameraSpeed = 0.03f;
	float camZMove = 0.0f;
	float camYMove = 0.0f;
	float m_fPitch = 0.0f;
	float m_fYaw = 0.0f;
	GLfloat m_fMouseSensitivity = 0.15f;
	GLfloat LastX = (float)0.0f;
	GLfloat LastY = (float)0.0f;

	GLfloat currentTime;
	static glm::mat4 MVP;
	static glm::mat4 Model;
};

