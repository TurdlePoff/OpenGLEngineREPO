#include "CubeMap.h"
#include "Camera.h"
#include "ShaderLoader.h"

CubeMap::CubeMap()
{
}


CubeMap::~CubeMap()
{
}

void CubeMap::InitCubeMap()
{
	m_program = ShaderLoader::GetInstance()->GetProgram((char*)"CubeMap");

	GLfloat verticesCubeMap[] = {
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		// Right Face
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,

		// Back Face
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		// Left Face
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		// Left Face
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,

		// Top Face
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,

		// Bottom Face
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
	};

	GLuint indicesCubeMap[] = {
		// Front Face
		0, 1, 2,
		0, 2, 3,
		// Right Face
		4, 5, 6,
		4, 6, 7,
		// Back Face
		8, 9, 10,
		8, 10, 11,
		// Left Face
		12, 13, 14,
		12, 14, 15,
		// Top Face	
		16, 17, 18,
		16, 18, 19,
		// Bottom Face
		20, 21, 22,
		20, 22, 23,
	};

	// Generating objects and texture
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

	m_iIndicesCount = sizeof(indicesCubeMap);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCubeMap), verticesCubeMap, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, m_iIndicesCount, indicesCubeMap, GL_STATIC_DRAW);

	//Setting the vertex attributes for the entity
	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Generating and biding the texture
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

	// Set Image via SOIL library
	int width, height;
	unsigned char* image = NULL;

	for (GLuint i = 0; i < 6; i++)
	{
		std::string fullPathName = "Resources/Sprites/CubeMap/";
		fullPathName.append(g_faces[i]);
		image = SOIL_load_image(fullPathName.c_str(), &width, &height, 0,
			SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}


	//Generate Mipmap
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Clear texture 2D
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::Render()
{
	glUseProgram(m_program);
	glBindVertexArray(m_vao);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glDisable(GL_CULL_FACE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

	glUniform1i(glGetUniformLocation(m_program, "cubeSampler"), 0);

	glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3());
	glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(1000.0f, 1000.0f, 1000.0f));
	/*glm::mat4 rotation = glm::rotate(glm::mat4(), glm::radians(glm::vec3().x), glm::vec3(1.0f, 0.0f, 0.0f));
	rotation = glm::rotate(glm::mat4(), glm::radians(glm::vec3().y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotation = glm::rotate(glm::mat4(), glm::radians(glm::vec3().z), glm::vec3(0.0f, 0.0f, 1.0f));*/

	glm::mat4 model = translation * scale;
	glm::mat4 MVP = Camera::GetInstance()->GetProjection() * Camera::GetInstance()->GetView() * model;
	glUniformMatrix4fv(glGetUniformLocation(m_program, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));

	glDrawElements(GL_TRIANGLES, m_iIndicesCount, GL_UNSIGNED_INT, 0);

	glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glBindVertexArray(0);
	glUseProgram(0);

}
