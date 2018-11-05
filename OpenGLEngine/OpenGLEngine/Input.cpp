//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2018 Media Design School
//
// File Name	: "Input.cpp"
// Description	: Input implementation file
// Author		: Vivian Ngo
// Mail			: vivian.ngo7572@mediadesign.school.nz
//

#include "Input.h"
#include "Camera.h"

Input* Input::s_pInputInstance = 0;

void LKeyboard_Down(unsigned char keyPressed, int x, int y);
void LKeyboard_Up(unsigned char keyPressed, int x, int y);
void LMouseClicked(int buttonPressed, int glutState, int x, int y);
void LMousePassiveMovement(int x, int y);
void LMouseScrollHold(int x, int y);
void LScrollCallback(int button, int glutState, int xOffset, int yOffset);
void LSpecialKeyPress(int key, int x, int y);
void LSpecialKeyRelease(int key, int x, int y);

/***********************
* GetInstance: Gets the instance of the Singleton Input class
* @author: Vivian Ngo
* @return: s_pInputInstance - Instance of the Input singleton class
***********************/
Input* Input::GetInstance()
{
	if (s_pInputInstance == 0)
	{
		s_pInputInstance = new Input();
	}
	return s_pInputInstance;
}

/***********************
* DestroyInstance: Destroys the instance of the Singleton Input class
* @author: Vivian Ngo
***********************/
void Input::DestroyInstance()
{
	if (s_pInputInstance != 0) // If there is an instance of this class
	{
		//Delete the instance
		delete s_pInputInstance;
		s_pInputInstance = nullptr;
	}
}

/***********************
* ~CInput Destructor
* @author: Vivian Ngo
***********************/
Input::~Input() 
{
	std::fill(MouseState, MouseState + 3, INPUT_RELEASED);
	std::fill(KeyState, KeyState + 255, INPUT_RELEASED);
}

/***********************
* Update: Updates input from first press to hold
* @author: Vivian Ngo
***********************/
void Input::Update()
{
	//Handle Keyboard states
	for (unsigned int i = 0; i < 255; ++i)
	{
		if (KeyState[i] == INPUT_FIRST_PRESS)
		{
			KeyState[i] = INPUT_HOLD;
		}
		if (KeyState[i] == INPUT_FIRST_RELEASE)
		{
			KeyState[i] = INPUT_RELEASED;
		}
	}

	//Handle Special Keyboard states
	for (unsigned int i = 0; i < 4; ++i)
	{
		if (SpecKeyState[i] = INPUT_FIRST_PRESS)
		{
			SpecKeyState[i] = INPUT_HOLD;
		}
		if (SpecKeyState[i] == INPUT_FIRST_RELEASE)
		{
			SpecKeyState[i] = INPUT_RELEASED;
		}
	}

	//Handle Mouse states
	for (unsigned int i = 0; i < 3; ++i)
	{
		if (MouseState[i] = INPUT_FIRST_PRESS)
		{
			MouseState[i] = INPUT_HOLD;
		}
		if (MouseState[i] == INPUT_FIRST_RELEASE)
		{
			MouseState[i] = INPUT_RELEASED;
		}
	}
}

bool Input::UpdateMousePicking(glm::vec3& _position, float _posRadius)
{
	//screen pos
	glm::vec2 normalizedScreenPos = glm::vec2(m_fMousePickingX, m_fMousePickingY);
	//screenposto ProjSpace
	glm::vec4 clipCoords = glm::vec4(normalizedScreenPos.x, normalizedScreenPos.y, -1.0f, 1.0f);
	//ProjSpace to eye space
	glm::mat4 invProjMat = glm::inverse(Camera::GetInstance()->GetProjection());
	glm::vec4 eyeCoords = invProjMat* clipCoords;
	eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

	//eyespaceto world space
	glm::mat4 invViewMat = glm::inverse(Camera::GetInstance()->GetView());
	glm::vec4 rayWorld = invViewMat* eyeCoords;
	rayDirection = glm::normalize(glm::vec3(rayWorld));

	glm::vec3 camPos = Camera::GetInstance()->GetCamPos();
	glm::vec3 dirVector = _position - camPos;

	float fA = glm::dot(rayDirection, rayDirection);
	float fB = 2.0f * glm::dot(dirVector, rayDirection);
	float fC = glm::dot(dirVector, dirVector) - _posRadius * _posRadius;
	float fD = fB * fB - 4.0f * fA * fC;

	float distance = glm::distance(camPos, _position);
	_position = rayDirection * distance + camPos;

	if (fD > 0.0f)
	{
		std::cout << "Picking!!" << std::endl;
		return true;
	}

	return false;
}

/***********************
* Keyboard_Down: Sets the KeyState of the given pressed key as INPUT_HOLD
* @author: Vivian Ngo
* @parameter: key - the character inputted
* @parameter: x - x position of the mouse at time character was pressed
* @parameter: y - y position of the mouse at time character was pressed
***********************/
void Input::Keyboard_Down(unsigned char key, int x, int y)
{
	KeyState[key] = INPUT_FIRST_PRESS;
}

/***********************
* Keyboard_Down: Sets the KeyState of the given released key as INPUT_RELEASED
* @author: Vivian Ngo
* @parameter: key - the character released
* @parameter: x - x position of the mouse at time character was released
* @parameter: y - y position of the mouse at time character was released
***********************/
void Input::Keyboard_Up(unsigned char key, int x, int y)
{
	KeyState[key] = INPUT_FIRST_RELEASE;
}

/***********************
* MouseClicked: Sets the MouseState as the currently held/released mouse button
* @author: Vivian Ngo
* @parameter: button - the mouse button clicked/released
* @parameter: glutState - the state of the mouse button
* @parameter: x - x position of the mouse
* @parameter: y - y position of the mouse
***********************/
void Input::MouseClicked(int button, int glutState, int x, int y)
{ 
	if (button < 3) 
	{ 
		MouseState[button] = (glutState == GLUT_DOWN) ? INPUT_FIRST_PRESS : INPUT_FIRST_RELEASE;
	} 
}

/***********************
* MousePassiveMovement: Allows screen to change pitch and yaw when navigating window
* @author: Vivian Ngo
* @parameter: x - x position of the mouse
* @parameter: y - y position of the mouse
***********************/
void Input::MousePassiveMovement(int x, int y)
{
	m_fMousePickingX = (2.0f * x) / (float)Utils::SCR_WIDTH - 1.0f;
	m_fMousePickingY = 1.0f - (2.0f * y) / (float)Utils::SCR_HEIGHT;

	m_fMouseX = (float)x;
	m_fMouseY = (float)y;
	if (FirstMouse == true)// Run only once to initialize the 'Last' vars
	{
		LastX = (GLfloat)x;
		LastY = (GLfloat)y;
		FirstMouse = false;
	}

	LastX = (GLfloat)x;
	LastY = (GLfloat)y;

	xPassiveOffset *= MouseSensitivity;
	yPassiveOffset *= MouseSensitivity;

	Yaw -= xPassiveOffset;
	Pitch -= yPassiveOffset;

	// Clamp 'Pitch' so screen doesn’t flip
	if (Pitch > 89.0f)
	{
		Pitch = 89.0f;
	}
	if (Pitch < -89.0f)
	{
		Pitch = -89.0f;
	}
}

/***********************
* MousePassiveMovement: Allows screen to change pitch and yaw when navigating window
* @author: Vivian Ngo
* @parameter: x - x position of the mouse
* @parameter: y - y position of the mouse
***********************/
void Input::MouseScrollHold(int x, int y)
{
	if (MouseState[0] == INPUT_HOLD)
	{
		if (FirstMouse == true)// Run only once to initialize the 'Last' vars
		{
			LastX = (GLfloat)x;
			LastY = (GLfloat)y;
			FirstMouse = false;
		}

		m_fMouseX = (float)x;
		m_fMouseY = (float)y;

		GLfloat xOffset = (GLfloat)x - LastX;
		GLfloat yOffset = (GLfloat)y - LastY;

		LastX = (GLfloat)x;
		LastY = (GLfloat)y;

		xOffset *= MouseSensitivity + 0.05f;
		yOffset *= MouseSensitivity + 0.05f;

		Yaw -= xOffset;

		Pitch -= yOffset;

		if (Pitch > 89.0f)
		{
			Pitch = 89.0f;
		}
		if (Pitch < -89.0f)
		{
			Pitch = -89.0f;
		}

		/*glm::vec3 frontVector(-cos(glm::radians(Pitch))*sin(glm::radians(Yaw)),
			sin(glm::radians(Pitch)),
			-cos(glm::radians(Pitch)) * cos(glm::radians(Yaw)));
		Camera::GetInstance()->SetCamFront(glm::normalize(frontVector));*/
	}
}

/***********************
* ScollCallback: Scroll window
* @author: Vivian Ngo
* @parameter: button - button 
* @parameter: glutState - scroll state
* @parameter: xOffset - x offset
* @parameter: yOffset - y offset
***********************/
void Input::ScrollCallback(int button, int glutState, int xOffset, int yOffset)
{
	Camera * c = Camera::GetInstance();
	float movement = 0.1f;
	//If player scrolls mouse
	if (glutState == 1 || glutState == -1)
	{
		glutState *= -1; //Reverse glutstate
		
		//Increase/decrease the field of view
		if (c->GetFOV() > 44.35f && c->GetFOV() < 46.2f)
		{
			c->SetFOV(c->GetFOV() + (movement * glutState));
		}
		else if (c->GetFOV() <= 44.4f)
		{
			c->SetFOV(44.4f);
		}
		else if (c->GetFOV() >= 46.1f)
		{
			c->SetFOV(46.1f);
		}
	}
}

/***********************
* SpecialKeyPress: Sets special key as pressed
* @author: Vivian Ngo
* @parameter: key - key pressed
* @parameter: x
* @parameter: y
***********************/
void Input::SpecialKeyPress(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_UP:
		{
			SpecKeyState[0] = INPUT_FIRST_PRESS;
			break;
		}
		case GLUT_KEY_DOWN:
		{
			SpecKeyState[1] = INPUT_FIRST_PRESS;
			break;

		}
		case GLUT_KEY_LEFT:
		{
			SpecKeyState[2] = INPUT_FIRST_PRESS;
			break;
		}
		case GLUT_KEY_RIGHT:
		{
			SpecKeyState[3] = INPUT_FIRST_PRESS;
			break;
		} 
	}
}

/***********************
* SpecialKeyRelease: Release special keys
* @author: Vivian Ngo
* @parameter: key - key pressed
* @parameter: x
* @parameter: y
***********************/
void Input::SpecialKeyRelease(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_UP:
		{
			SpecKeyState[0] = INPUT_FIRST_RELEASE;
			break;
		}
		case GLUT_KEY_DOWN:
		{
			SpecKeyState[1] = INPUT_FIRST_RELEASE;
			break;

		}
		case GLUT_KEY_LEFT:
		{
			SpecKeyState[2] = INPUT_FIRST_RELEASE;
			break;
		}
		case GLUT_KEY_RIGHT:
		{
			SpecKeyState[3] = INPUT_FIRST_RELEASE;
			break;
		}
	}
}

void Input::HandleInput()
{
	//Keyboard Input
	glutKeyboardFunc(LKeyboard_Down);
	glutKeyboardUpFunc(LKeyboard_Up);
	glutSpecialFunc(LSpecialKeyPress);
	glutSpecialUpFunc(LSpecialKeyRelease);

	//Mouse Input
	glutMouseFunc(LMouseClicked);
	glutPassiveMotionFunc(LMousePassiveMovement);
	glutMotionFunc(LMouseScrollHold);
	glutMouseWheelFunc(LScrollCallback);
}

/***********************
* LKeyboard_Down: Local function to handle OpenGL glutKeyboardFunc input
***********************/
void LKeyboard_Down(unsigned char keyPressed, int x, int y)
{
	Input::GetInstance()->Keyboard_Down(keyPressed, x, y);
}

/***********************
* LKeyboard_Up: Local function to handle OpenGL glutKeyboardUpFunc input
***********************/
void LKeyboard_Up(unsigned char keyPressed, int x, int y)
{
	Input::GetInstance()->Keyboard_Up(keyPressed, x, y);
}

/***********************
* LMouseClicked: Local function to handle OpenGL glutMouseFunc input
***********************/
void LMouseClicked(int buttonPressed, int glutState, int x, int y)
{
	Input::GetInstance()->MouseClicked(buttonPressed, glutState, x, y);
}

/***********************
* LMousePassiveMovement: Local function to handle OpenGL glutPassiveMotionFunc input
***********************/
void LMousePassiveMovement(int x, int y)
{
	Input::GetInstance()->MousePassiveMovement(x, y);
}

/***********************
* LMouseScrollHold: Local function to handle OpenGL glutMotionFunc input
***********************/
void LMouseScrollHold(int x, int y)
{
	Input::GetInstance()->MouseScrollHold(x, y);
}

/***********************
* LScrollCallback: Local function to handle OpenGL glutMouseWheelFunc input
***********************/
void LScrollCallback(int button, int glutState, int xOffset, int yOffset)
{
	Input::GetInstance()->MouseClicked(button, glutState, xOffset, yOffset);
}

/***********************
* LSpecialKeyPress: Local function to handle OpenGL glutSpecialFunc input
***********************/
void LSpecialKeyPress(int key, int x, int y)
{
	Input::GetInstance()->SpecialKeyPress(key, x, y);
}

/***********************
* LSpecialKeyRelease: Local function to handle OpenGL glutSpecialUpFunc input
***********************/
void LSpecialKeyRelease(int key, int x, int y)
{
	Input::GetInstance()->SpecialKeyRelease(key, x, y);
}
