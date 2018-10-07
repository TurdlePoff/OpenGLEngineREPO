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

//Static variables
GLfloat Input::MouseSensitivity = 0.05f;
GLfloat Input::Yaw = 0.0f;
GLfloat Input::Pitch = 0.0f;
GLfloat Input::Roll = 0.0f;
GLfloat Input::LastX = (float)Utils::SCR_WIDTH;
GLfloat Input::LastY = (float)Utils::SCR_HEIGHT;
float Input::LastScrollY = 365.0f;
bool Input::FirstMouse = true;
float Input::m_fMouseX = 0.0f;
float Input::m_fMouseY = 0.0f;

unsigned int Input::MouseState[3];
unsigned int Input::KeyState[255];
unsigned int Input::SpecKeyState[4];
unsigned int Input::SpaceState[255];

GLfloat xOffset;
GLfloat yOffset;

/***********************
* GetInstance: Gets the instance of the Singleton Input class
* @author: Vivian Ngo
* @date: 08/05/18
* @return: s_pInputInstance - Instance of the Input singleton class
***********************/
Input * Input::GetInstance()
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
* @date: 08/05/18
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
* @date: 08/05/18
***********************/
Input::~Input() {}

/***********************
* Keyboard_Down: Sets the KeyState of the given pressed key as INPUT_HOLD
* @author: Vivian Ngo
* @date: 08/05/18
* @parameter: key - the character inputted
***********************/
void Input::Keyboard_Down(unsigned char key, int x, int y)
{ 
	if (key != ' ')
	{
		KeyState[key] = INPUT_HOLD;
	}
	else
	{
  		SpaceState[key] = INPUT_HOLD;
	}
}

/***********************
* Keyboard_Down: Sets the KeyState of the given released key as INPUT_RELEASED
* @author: Vivian Ngo 
* @date: 08/05/18
* @parameter: key - the character released
***********************/
void Input::Keyboard_Up(unsigned char key, int x, int y)
{

	if (SpaceState[key] == INPUT_HOLD)
	{
		SpaceState[key] = INPUT_RELEASED;
	}
	
	if(KeyState[key] == INPUT_HOLD)
	{
		KeyState[key] = INPUT_RELEASED;

	}
}

/***********************
* MouseClicked: Sets the MouseState as the currently held/released mouse button
* @author: Vivian Ngo
* @date: 08/05/18
* @parameter: button - the mouse button clicked/released
* @parameter: glutState - the state of the mouse button
***********************/
void Input::MouseClicked(int button, int glutState, int x, int y)
{ 
	if (button < 3) 
	{ 
		MouseState[button] = (glutState == GLUT_DOWN) ? INPUT_HOLD : INPUT_RELEASED;
	} 
}

/***********************
* MousePassiveMovement: Allows screen to change pitch and yaw when navigating window
* @author: Vivian Ngo
* @date: 29/05/18
* @parameter: x - x position of the mouse
* @parameter: y - y position of the mouse
***********************/
void Input::MousePassiveMovement(int x, int y)
{
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

	xOffset *= MouseSensitivity;
	yOffset *= MouseSensitivity;

	Yaw -= xOffset;
	Pitch -= yOffset;

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
* @date: 29/05/18
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
	}
}

/***********************
* ScollCallback: Scroll window
* @author: Vivian Ngo
* @date: 29/05/18
* @parameter: button - button 
* @parameter: glutState - scroll state
* @parameter: xOffset - x offset
* @parameter: yOffset - y offset
***********************/
void Input::ScollCallback(int button, int glutState, int xOffset, int yOffset)
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
* @date: 08/05/18
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
			SpecKeyState[0] = INPUT_HOLD;
			break;
		}
		case GLUT_KEY_DOWN:
		{
			SpecKeyState[1] = INPUT_HOLD;
			break;

		}
		case GLUT_KEY_LEFT:
		{
			SpecKeyState[2] = INPUT_HOLD;
			break;
		}
		case GLUT_KEY_RIGHT:
		{
			SpecKeyState[3] = INPUT_HOLD;
			break;
		} 
	}
}

/***********************
* SpecialKeyRelease: Release special keys
* @author: Vivian Ngo
* @date: 08/05/18
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
			SpecKeyState[0] = INPUT_RELEASED;
			break;
		}
		case GLUT_KEY_DOWN:
		{
			SpecKeyState[1] = INPUT_RELEASED;
			break;

		}
		case GLUT_KEY_LEFT:
		{
			SpecKeyState[2] = INPUT_RELEASED;
			break;
		}
		case GLUT_KEY_RIGHT:
		{
			SpecKeyState[3] = INPUT_RELEASED;
			break;
		}
	}
}

void Input::ProcessInput()
{
	//Keyboard Input
	glutKeyboardFunc(Keyboard_Down);
	glutKeyboardUpFunc(Keyboard_Up);
	glutSpecialFunc(SpecialKeyPress);
	glutSpecialUpFunc(SpecialKeyRelease);

	//Mouse Input
	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MousePassiveMovement);
	glutMotionFunc(MouseScrollHold);
	glutMouseWheelFunc(ScollCallback);
}
