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
//#include "Input.h"
//#include "Camera.h"
//#include "Clock.h"
//#include "SceneManager.h"
//#include "ShaderLoader.h"
#include <vld.h> 

void Init();
void Render(void);
void Process(void);
void Exit();

//std::map<std::string, int> hi;

/***********************
* Init: Initialises items for the application
* @author: Vivian Ngo
* @date: 08/05/18
***********************/
void Init()
{
	srand((unsigned int)time(NULL));
	//hi["shoo"] = 1;

	/*ShaderLoader::GetInstance()->CreateProgram((char*)"Texture", 
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
*/
	/*ShaderLoader::GetInstance()->CreateProgram((char*)"CubeMap",
		(char*)"Resources/Shaders/CMapVertexShader.vs",
		(char*)"Resources/Shaders/CMapFragmentShader.fs");*/

	//SceneManager::GetInstance()->InitScenes();
}

/***********************
* Render: Renders objects of the application
* @author: Vivian Ngo
* @date: 08/05/18
***********************/
void Render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//SceneManager::GetInstance()->RenderScene();
	glutSwapBuffers();
}


/***********************
* Update: Updates the application
* @author: Vivian Ngo
* @date: 08/05/18
***********************/
void Process(void)
{
	//Clock::Update();
	//std::cout << "SHOO: " <<  hi.find("shoo")->second << std::endl;
	//SceneManager::GetInstance()->ProcessScene(Clock::GetInstance()->GetDeltaTick());
	//Camera::GetInstance()->Process(Clock::GetInstance()->GetDeltaTick());
	glutPostRedisplay(); //render function is called
}

/***********************
* Exit: Destroys all singleton instances of the application
* @author: Vivian Ngo
* @date: 08/05/18
***********************/
void Exit()
{
	//SceneManager::GetInstance()->DestroyInstance();
	//Camera::GetInstance()->DestroyInstance();
	//Clock::GetInstance()->DestroyInstance();
	ShaderLoader::DestroyInstance();
	//Input::GetInstance()->DestroyInstance();

}


/***********************
* main: main for OpenGL application
* @author: Vivian Ngo
* @date: 08/05/18
***********************/
int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	//OpenGL window
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(485, 50);
	//glutInitWindowPosition(2300, 50);

	glutInitWindowSize(Utils::SCR_WIDTH, Utils::SCR_HEIGHT);
	glutCreateWindow("3DProject");
	glClearColor(0.2f, 0.5f, 1.0f, 1.0f);	//clear blue

	glewInit();
	Init();

	//Input::ProcessInput();

	glutDisplayFunc(Render);
	glutIdleFunc(Process);
	glutMainLoop();

	glutCloseFunc(Exit);
	return 0;
}