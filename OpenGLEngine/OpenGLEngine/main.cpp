/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name	: "main.cpp"
* Description	: main.cpp OpenGL Project
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "Utility.h"
#include "TextLabel.h"
#include "Entity.h"
#include "Input.h"
#include "Camera.h"
#include "Clock.h"
#include "SceneManager.h"
#include "ShaderLoader.h"
#include <vld.h> 

void Init();
void Render(void);
void Process(void);
void Exit();

/***********************
* Init: Initialises items for the application
* @author: Vivian Ngo
***********************/
void Init()
{
	srand((unsigned int)time(NULL));

	ShaderLoader::GetInstance()->CreateProgram((char*)"Texture", 
		(char*)"Resources/Shaders/TextureVertexShader.vs", 
		(char*)"Resources/Shaders/TextureFragmentShader.fs");
	ShaderLoader::GetInstance()->CreateProgram((char*)"Text", 
		(char*)"Resources/Shaders/TextVertexShader.vs", 
		(char*)"Resources/Shaders/TextFragmentShader.fs");
	ShaderLoader::GetInstance()->CreateProgram((char*)"Terrain",
		(char*)"Resources/Shaders/HeightmapVertexShader.vs",
		(char*)"Resources/Shaders/HeightmapFragmentShader.fs");
	ShaderLoader::GetInstance()->CreateProgram((char*)"Plain",
		(char*)"Resources/Shaders/PlainVertexShader.vs",
		(char*)"Resources/Shaders/PlainFragmentShader.fs");
	ShaderLoader::GetInstance()->CreateProgram((char*)"Star",
		(char*)"Resources/Shaders/StarVertexShader.vs",
		(char*)"Resources/Shaders/StarFragmentShader.fs", 
		(char*)"Resources/Shaders/StarGeoShader.gs");
	/*ShaderLoader::GetInstance()->CreateProgram((char*)"CubeMap",
		(char*)"Resources/Shaders/CMapVertexShader.vs",
		(char*)"Resources/Shaders/CMapFragmentShader.fs");*/

	SceneManager::GetInstance()->InitScenes();
}

/***********************
* Render: Renders objects of the application
* @author: Vivian Ngo
***********************/
void Render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	SceneManager::GetInstance()->RenderScene();
	glutSwapBuffers();
}


/***********************
* Update: Updates the application
* @author: Vivian Ngo
***********************/
void Process(void)
{
	Clock::GetInstance()->Update();
	SceneManager::GetInstance()->ProcessScene(Clock::GetInstance()->GetDeltaTick());
	Camera::GetInstance()->Process(Clock::GetInstance()->GetDeltaTick());
	Input::GetInstance()->Update();

	glutPostRedisplay(); //render function is called

}

/***********************
* Exit: Destroys all singleton instances of the application
* @author: Vivian Ngo
***********************/
void Exit()
{
	SceneManager::GetInstance()->DestroyInstance();
	Camera::GetInstance()->DestroyInstance();
	Clock::GetInstance()->DestroyInstance();
	Input::GetInstance()->DestroyInstance();
	exit(0);
}


/***********************
* main: main for OpenGL application
* @author: Vivian Ngo
***********************/
int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	//OpenGL window
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 50);

	//glutInitWindowPosition(485, 50);
	//glutInitWindowPosition(2300, 50);

	glutInitWindowSize(Utils::SCR_WIDTH, Utils::SCR_HEIGHT);
	glutCreateWindow("3DProject");
	glClearColor(0.2f, 0.5f, 1.0f, 1.0f);	//clear blue

	glewInit();
	Init();

	Input::GetInstance()->HandleInput();


	glutDisplayFunc(Render);
	glutIdleFunc(Process);
	glutCloseFunc(Exit);
	glutMainLoop();

	return 0;
}