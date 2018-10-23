 /****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "ShaderLoader.cpp"
* Description	: ShaderLoader implementation file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "ShaderLoader.h" 

#include <iostream>
#include <fstream>
#include <vector>

std::shared_ptr<ShaderLoader> ShaderLoader::s_pSLInstance = 0;
//std::map<std::string, std::pair<std::pair<GLuint, GLuint>, GLuint>> ShaderLoader::m_shaderMap;
//std::map<std::string, std::pair<std::pair<GLuint, GLuint>, std::pair<GLuint, GLuint>>>  ShaderLoader::m_geoShaderMap;

/***********************
* ShaderLoader: ShaderLoader constructor
* @author: Vivian Ngo
***********************/
std::shared_ptr<ShaderLoader> ShaderLoader::GetInstance()
{
	if (s_pSLInstance == 0)
	{
		s_pSLInstance = std::make_shared<ShaderLoader>();
	}
	return s_pSLInstance;
}

/***********************
* DestroyInstance: Destroy Instance
* @author: Vivian Ngo
***********************/
void ShaderLoader::DestroyInstance()
{
	if (s_pSLInstance != 0) // If there is an instance of this class
	{
		//Delete the instance
		delete s_pSLInstance.get();
		s_pSLInstance = nullptr;
	}
}

/***********************
* ShaderLoader: ShaderLoader Constructor
* @author: Vivian Ngo
***********************/
ShaderLoader::ShaderLoader(void) {}

/***********************
* ~ShaderLoader: ShaderLoader destructor
* @author: Vivian Ngo
***********************/
ShaderLoader::~ShaderLoader(void) {
	//m_shaderMap.clear();
	//m_geoShaderMap.clear();

}

/***********************
* ReadShader: Read Shader file
* @author: Vivian Ngo
* @parameter: filename - file to read from
***********************/
std::string ShaderLoader::ReadShader(char *filename)
{
	std::string shaderCode;
	std::ifstream file(filename, std::ios::in);

	if (!file.good()) {
		std::cout << "Can't read file " << filename << std::endl;
		std::terminate();
	}

	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)file.tellg());
	file.seekg(0, std::ios::beg);
	file.read(&shaderCode[0], shaderCode.size());
	file.close();
	return shaderCode;
}

/***********************
* CreateShader: Creates Shader file
* @author: Vivian Ngo
* @parameter: shaderType - type of shader
* @parameter: source - shader source
* @parameter: shaderName - name of shader
***********************/
GLuint ShaderLoader::CreateShader(GLenum shaderType, std::string
	source, char* shaderName)
{
	int compile_result = 0;

	GLuint shader = glCreateShader(shaderType);
	const char *shader_code_ptr = static_cast<const char*>(source.c_str());
	const int shader_code_size = static_cast<const int>(source.size());

	glShaderSource(shader, 1, &shader_code_ptr, &shader_code_size);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);

	//check for errors
	if (compile_result == GL_FALSE)
	{
		int info_log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> shader_log(info_log_length);
		glGetShaderInfoLog(shader, info_log_length, NULL, &shader_log[0]);
		std::cout << "ERROR compiling shader: " << shaderName << std::endl << &shader_log[0] << std::endl;
		return 0;
	}
	return shader;
}

/***********************
* CreateProgram: Creates Shader program
* @author: Vivian Ngo
* @parameter: vertexShaderFilename - vertex shader file
* @parameter: fragmentShaderFilename - fragment shader file
***********************/
void ShaderLoader::CreateProgram(char* shaderName, char* VertexShaderFilename,
	char* FragmentShaderFilename)
{
	//read the shader files and save the code
	std::string vertex_shader_code = ReadShader(VertexShaderFilename);
	std::string fragment_shader_code = ReadShader(FragmentShaderFilename);

	if (!m_shaderMap.empty())
	{
		for (auto it = m_shaderMap.begin(); it != m_shaderMap.end(); ++it)
		{
			if (it->first == shaderName)
			{
				std::cout << shaderName << " shader already exists." << std::endl;
				return;
			}
		}
	}

	GLuint vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_shader_code, (char*)"vertex shader");
	GLuint fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code, (char*)"fragment shader");

	int link_result = 0;
	//create the program handle, attatch the shaders and link it
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	//check for link errors
	if (link_result == GL_FALSE)
	{

		int info_log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
		std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
	}

	/*m_shaderMap[vertexShaderFilename] = std::make_pair(vertex_shader, program);
	m_shaderMap[fragmentShaderFilename] = std::make_pair(fragment_shader, program);*/
	std::cout << "ShaderLoader: " << shaderName << " shader created." << std::endl;

	m_shaderMap[shaderName] = std::make_pair(std::make_pair(vertex_shader, fragment_shader), program);
}

/***********************
* CreateProgram: Creates a GeoShader program
* @author: Vivian Ngo
* @parameter: vertexShaderFilename - vertex shader file
* @parameter: fragmentShaderFilename - fragment shader file
***********************/
void ShaderLoader::CreateProgram(char* shaderName, char* VertexShaderFilename,
	char* FragmentShaderFilename, char* GeoShaderFileName)
{
	//read the shader files and save the code
	std::string vertex_shader_code = ReadShader(VertexShaderFilename);
	std::string fragment_shader_code = ReadShader(FragmentShaderFilename);
	std::string geo_shader_code = ReadShader(GeoShaderFileName);

	if (!m_shaderMap.empty())
	{
		for (auto it = m_shaderMap.begin(); it != m_shaderMap.end(); ++it)
		{
			if (it->first == shaderName)
			{
				std::cout << shaderName << " shader already exists." << std::endl;
				return;
			}
		}
	}

	GLuint vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_shader_code, (char*)"vertex shader");
	GLuint fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code, (char*)"fragment shader");
	GLuint geo_shader = CreateShader(GL_GEOMETRY_SHADER, geo_shader_code, (char*)"geo shader");

	int link_result = 0;
	//create the program handle, attatch the shaders and link it
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glAttachShader(program, geo_shader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	//check for link errors
	if (link_result == GL_FALSE)
	{

		int info_log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
		std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
	}

	/*m_shaderMap[vertexShaderFilename] = std::make_pair(vertex_shader, program);
	m_shaderMap[fragmentShaderFilename] = std::make_pair(fragment_shader, program);*/
	std::cout << "ShaderLoader: " << shaderName << " shader created." << std::endl;

	m_geoShaderMap[shaderName] = std::make_pair(std::make_pair(vertex_shader, fragment_shader), std::make_pair(geo_shader, program));
}

GLuint ShaderLoader::GetProgram(char * shaderName)
{
	if (!m_shaderMap.empty())
	{
		for (auto it = m_shaderMap.begin(); it != m_shaderMap.end(); ++it)
		{
			if (it->first == shaderName)
			{
				return it->second.second;
			}
		}
	}

	if (!m_geoShaderMap.empty())
	{
		for (auto it = m_geoShaderMap.begin(); it != m_geoShaderMap.end(); ++it)
		{
			if (it->first == shaderName)
			{
				return it->second.second.second;
			}
		}
	}

	std::string bleh = " shader doesn't exist.";
	throw std::invalid_argument(shaderName + bleh);
	return 0;
}
