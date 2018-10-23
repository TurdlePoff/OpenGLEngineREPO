/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name	: "Camera.cpp"
* Description	: Camera implementation file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "Camera.h"
#include "Input.h"

Camera* Camera::s_pCameraInstance = 0;
glm::vec3 Camera::cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 Camera::cameraPos = glm::vec3(0.0f, 0.0f, 100.0f);
glm::vec3 Camera::cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 Camera::cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 Camera::cameraRot = glm::vec3(0.0f, 0.0f, 0.0f);

glm::mat4 Camera::view = glm::mat4();		//view
glm::mat4 Camera::projection = glm::mat4();	//projection

glm::mat4 Camera::MVP = glm::mat4(); // field of view
glm::mat4 Camera::Model = glm::mat4();
float Camera::m_fov = 45.0f; // field of view

/***********************
* Camera: Camera constructor
* @author: Vivian Ngo
***********************/
Camera::Camera() 
{
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	projection = glm::perspective(45.0f, (float)Utils::SCR_WIDTH / (float)Utils::SCR_HEIGHT, 0.1f, 10000.0f);

	//projection = glm::ortho(0, Utils::SCR_WIDTH, 0, Utils::SCR_HEIGHT, 0.1f, 1000.0f);
	//projection = glm::ortho(-(GLfloat)Utils::SCR_WIDTH/2, (GLfloat)Utils::SCR_WIDTH/2, (GLfloat)Utils::SCR_HEIGHT/2, 0.1f, 10000.0f);
}

/***********************
* GetInstance: Gets the instance of the camera singleton class
* @author: Vivian Ngo
* @return: s_pCameraInstance - Instance of the Camera singleton class
***********************/
Camera* Camera::GetInstance()
{
	if (s_pCameraInstance == 0)
	{
		s_pCameraInstance = new Camera();
	}
	return s_pCameraInstance;
}

/***********************
* DestroyInstance: Destroys the instance of the camera singleton class if there is one
* @author: Vivian Ngo
***********************/
void Camera::DestroyInstance()
{
	if (s_pCameraInstance != 0) // If there is an instance of this class
	{
		//Delete the instance
		delete s_pCameraInstance;
		s_pCameraInstance = nullptr;
	}
}

///***********************
//* GetCam: Gets the camera
//* @author: Vivian Ngo
//* @return: projection view of camera object
//***********************/
//glm::mat4 Camera::GetCam()
//{
//	view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
//	projection = glm::perspective(45.0f, (float)Utils::SCR_WIDTH / (float)Utils::SCR_HEIGHT, 0.1f, 1000.0f);
//
//	//Calculate the model to be displayed onto the screen
//	glm::mat4 translate = glm::translate(glm::mat4(), cameraPos);
//	glm::mat4 rotation;
//	rotation = glm::rotate(glm::mat4(), glm::radians(cameraRot.x), glm::vec3(1, 0, 0));
//	rotation = glm::rotate(glm::mat4(), glm::radians(cameraRot.y), glm::vec3(0, 1, 0));
//	rotation = glm::rotate(glm::mat4(), glm::radians(cameraRot.z), glm::vec3(0, 0, 1));
//
//	return projection * view * (rotation * translate);
//}

/*********************** 
* Process: Process camera
* @author: Vivian Ngo
***********************/
void Camera::Process(float _deltaTick)
{
	//view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	CameraMovement(_deltaTick);
}

/***********************
* SetFOV: Sets the field of view
* @author: Vivian Ngo
* @parameter: _camSpeed speed of the camera
***********************/
void Camera::CameraMovement(float _deltaTick)
{
	//cameraSpeed = 10.0f;
	if (Input::GetInstance()->KeyState['U' ] == INPUT_HOLD || Input::GetInstance()->KeyState['u'] == INPUT_HOLD)
	{
		cameraPos.y += cameraSpeed * _deltaTick;
	}
	if (Input::GetInstance()->KeyState['J'] == INPUT_HOLD || Input::GetInstance()->KeyState['j'] == INPUT_HOLD)
	{
		cameraPos.y -= cameraSpeed * _deltaTick; //-= cameraSpeed * cameraUp;
	}
	if (Input::GetInstance()->KeyState['H'] == INPUT_HOLD || Input::GetInstance()->KeyState['h'] == INPUT_HOLD)
	{
		cameraPos.x -= cameraSpeed *_deltaTick;
	}
	if (Input::GetInstance()->KeyState['K'] == INPUT_HOLD || Input::GetInstance()->KeyState['k'] == INPUT_HOLD)
	{
		cameraPos.x += cameraSpeed * _deltaTick;
	}
	if (Input::GetInstance()->KeyState['Y'] == INPUT_HOLD || Input::GetInstance()->KeyState['y'] == INPUT_HOLD)
	{
		cameraPos.z -= cameraSpeed * _deltaTick;
	}
	if (Input::GetInstance()->KeyState['I'] == INPUT_HOLD || Input::GetInstance()->KeyState['i'] == INPUT_HOLD)
	{
		cameraPos.z += cameraSpeed * _deltaTick;
	}
}

/***********************
* SetFOV: Sets the field of view
* @author: Vivian Ngo
* @parameter: _fov to change
***********************/
void Camera::SetFOV(float _fov)
{
	m_fov = _fov;
}

/***********************
* GetFOV: Gets the field of view
* @author: Vivian Ngo
* @return: _fov
***********************/
float Camera::GetFOV()
{
	return m_fov;
}

/***********************
* SetCamFront: Sets the camera front
* @author: Vivian Ngo
* @parameter: _cFront - the coordinates of the camera front to change to
***********************/
void Camera::SetCamFront(glm::vec3 _cFront)
{
	cameraFront = _cFront;
}

/***********************
* SetCamUp: Sets camera up
* @author: Vivian Ngo
* @parameter: _cUp - the coordinates of camera up to change to
***********************/
void Camera::SetCamUp(glm::vec3 _cUp)
{
	cameraUp = _cUp;
}

/***********************
* GetCamFront: Gets the camera front
* @author: Vivian Ngo
* @return: cameraFront - the camera front
***********************/
glm::vec3 Camera::GetCamFront()
{
	return cameraFront;
}

/***********************
* SetCamPos: Sets the camera position
* @author: Vivian Ngo
* @parameter: _cPos - the coordinates of the camera position to change to
***********************/
void Camera::SetCamPos(glm::vec3 _cPos)
{
	cameraPos = _cPos;
}

/***********************
* GetCamPos: Gets the camera's position
* @author: Vivian Ngo
* @return: cameraPos - the camera's position
***********************/
glm::vec3 Camera::GetCamPos()
{
	return cameraPos;
}

/***********************
* GetCamUp: Gets the camera up coordinates
* @author: Vivian Ngo
* @return: cameraUp - the camera up coordinate
***********************/
glm::vec3 Camera::GetCamUp()
{
	return cameraUp;
}

/***********************
* SetCamSpeed: Sets the speed of the camera
* @author: Vivian Ngo
* @parameter: _cSpeed - the camera speed to change to
***********************/
void Camera::SetCamSpeed(float _cSpeed)
{
	cameraSpeed = _cSpeed;
}

/***********************
* GetCamSpeed: Gets the camera speed
* @author: Vivian Ngo
* @return: cameraSpeed - the camera speed
***********************/
float Camera::GetCamSpeed()
{
	return cameraSpeed;
}

glm::mat4 Camera::GetView()
{
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	return view;
}

glm::mat4 Camera::GetRotation()
{

	glm::mat4 rotation;
	rotation = glm::rotate(glm::mat4(), glm::radians(cameraRot.x), glm::vec3(1, 0, 0));
	rotation = glm::rotate(glm::mat4(), glm::radians(cameraRot.y), glm::vec3(0, 1, 0));
	rotation = glm::rotate(glm::mat4(), glm::radians(cameraRot.z), glm::vec3(0, 0, 1));
	return rotation;
}
