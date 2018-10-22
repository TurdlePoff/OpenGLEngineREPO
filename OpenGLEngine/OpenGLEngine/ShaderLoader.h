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
	static std::shared_ptr<ShaderLoader> GetInstance();
	static void DestroyInstance();

	ShaderLoader(void);
	~ShaderLoader(void);
	void CreateProgram(char* shaderName, char* VertexShaderFilename,
		char* FragmentShaderFilename);
	void CreateProgram(char* shaderName, char* VertexShaderFilename,
		char* FragmentShaderFilename, char* GeoShaderFileName);

	GLuint GetProgram(char* shaderName);

private:
	static std::shared_ptr<ShaderLoader> s_pSLInstance;
	std::map<std::string, std::pair<std::pair<GLuint, GLuint>, GLuint>> m_shaderMap;
	std::map<std::string, std::pair<std::pair<GLuint, GLuint>, std::pair<GLuint, GLuint>>> m_geoShaderMap;

	std::string ReadShader(char *filename);
	GLuint CreateShader(GLenum shaderType, std::string source, char* shaderName);
};
