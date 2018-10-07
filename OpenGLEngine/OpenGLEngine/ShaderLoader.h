#pragma once
/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name	: "ShaderLoader.h"
* Description	: ShaderLoader declaration file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "Utility.h"

class ShaderLoader
{

public:
	static ShaderLoader* GetInstance();
	static void DestroyInstance();

	ShaderLoader(void);
	~ShaderLoader(void);
	void CreateProgram(char* shaderName, char* VertexShaderFilename,
		char* FragmentShaderFilename);
	static GLuint GetProgram(char* shaderName);

private:
	static ShaderLoader* s_pSLInstance;
	static std::map<std::string, std::pair<std::pair<GLuint, GLuint>, GLuint>> m_shaderMap;

	std::string ReadShader(char *filename);
	GLuint CreateShader(GLenum shaderType, std::string source, char* shaderName);
};
