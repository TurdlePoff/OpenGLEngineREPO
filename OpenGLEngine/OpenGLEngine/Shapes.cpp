/************************************                                                         ****************
* Bachelor of Software Engineering                                     
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "Shapes.cpp"
* Description	: Shapes implementation file
*					Available shapes
*					- 2D quad
*					- 3D cube
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/
#include "Shapes.h"

Shapes::Shapes()
{
}


Shapes::~Shapes()
{
}                         

void Shapes::Init(const char * _imageType, EShapes _shape)
{
	m_program = ShaderLoader::GetInstance()->GetProgram((char*)"Texture");

	m_vPos = glm::vec3(0.0f, 0.0f, -1.0f);
	m_vRot = glm::vec3();
	m_vScale = glm::vec3(1.0f, 1.0f, 1.0f);

	//Generating Buffers and Arrays
	glGenVertexArrays(1, &m_vao);					//Vert Array
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);                        //Vert Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glGenBuffers(1, &m_ebo);						//Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

	switch (_shape)
	{
	case QUAD:
	{
		//Setting Buffer Data
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticesSquare), verticesSquare, GL_STATIC_DRAW);			//VBO Buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesSquare), indicesSquare, GL_STATIC_DRAW);    //EBO Buffer
		m_iIndicesCount = sizeof(indicesSquare);
		break;
	}
	case CUBE:
	{
		//Setting Buffer Data
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);			//VBO Buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCube), indicesCube, GL_STATIC_DRAW);    //EBO Buffer
		m_iIndicesCount = sizeof(indicesCube);
		break;
	}
	}

	//Setting the vertex attributes for the entity
	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);                                                                                              

	//Color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	                
	//Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//Generating and biding the texture
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	//Loading the image
	unsigned char* image = SOIL_load_image(_imageType, &m_iWidth, &m_iHeight, 0, SOIL_LOAD_RGBA);
	//Defining the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Generating the texture, freeing up the data and binding it
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Shapes::Init(const char * _imageType, float _radius)
{
	m_program = ShaderLoader::GetInstance()->GetProgram((char*)"Texture");

	m_vPos = glm::vec3(0.0f, 0.0f, -1.0f);
	m_vRot = glm::vec3();
	m_vScale = glm::vec3(1.0f, 1.0f, 1.0f);


	m_radius = _radius; //default 1

	const int sections = 20;
	const int vertexAttrib = 8;
	const int indexPerQuad = 6;

	double phi = 0;
	double theta = 0;

	float vertices[(sections) * (sections)* vertexAttrib];
	int offset = 0;
	for (int i = 0; i < sections; i++)
	{
		theta = 0;

		for (int j = 0; j < sections; j++)
		{
			float x = cos(phi) * sin(theta);
			float y = cos(theta);
			float z = sin(phi) * sin(theta);

			vertices[offset++] = x * m_radius;
			vertices[offset++] = y * m_radius;
			vertices[offset++] = z * m_radius;

			vertices[offset++] = x;
			vertices[offset++] = y;
			vertices[offset++] = z;

			vertices[offset++] = (float)i / (sections - 1);
			vertices[offset++] = (float)j / (sections - 1);

			theta += (M_PI / (sections - 1));
		}

		phi += (2 * M_PI) / (sections - 1);
	}


	GLuint indices[(sections) * (sections)* indexPerQuad];
	offset = 0;
	for (int i = 0; i < sections; i++)
	{
		for (int j = 0; j < sections; j++)
		{
			indices[offset++] = (((i + 1) % sections) * sections) + ((j + 1) % sections);
			indices[offset++] = (((i + 1) % sections) * sections) + (j);
			indices[offset++] = (i * sections) + (j);

			indices[offset++] = (i * sections) + ((j + 1) % sections);
			indices[offset++] = (((i + 1) % sections) * sections) + ((j + 1) % sections);
			indices[offset++] = (i * sections) + (j);
		}
	}

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	m_iIndicesCount = sizeof(indices) / sizeof(GLuint);

	//Generating and biding the texture
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	//Loading the image
	unsigned char* image = SOIL_load_image(_imageType, &m_iWidth, &m_iHeight, 0, SOIL_LOAD_RGBA);
	//Defining the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Generating the texture, freeing up the data and binding it
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Shapes::Render()
{
	glUseProgram(m_program);
	glBindVertexArray(m_vao);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	if (m_eShape == QUAD)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		//Enabling back face culling
		glCullFace(GL_BACK);        //Face to cull
		glFrontFace(GL_CW);         //Defining the front face via vertex initialization order
		glEnable(GL_CULL_FACE);     //Enabling back face culling
	}
	else
	{
		/*glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);
		glEnable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);*/
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);
		glEnable(GL_CULL_FACE);
	}

	glEnable(GL_BLEND);     //Enabling back face culling
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glUniform1i(glGetUniformLocation(m_program, "tex"), 0);

	//ModelMatrix
	glm::mat4 translation = glm::translate(glm::mat4(), m_vPos);
	glm::mat4 rotation = glm::rotate(glm::mat4(), glm::radians(m_vRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
	rotation = glm::rotate(glm::mat4(), glm::radians(m_vRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotation = glm::rotate(glm::mat4(), glm::radians(m_vRot.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scale = glm::scale(glm::mat4(), m_vScale);

	glm::mat4 Model = translation * rotation * scale;
	glm::mat4 MVP = Camera::GetInstance()->GetProjection() * Camera::GetInstance()->GetView() * Model;

	glUniformMatrix4fv(glGetUniformLocation(m_program, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	glDrawElements(GL_TRIANGLES, m_iIndicesCount, GL_UNSIGNED_INT, 0);

	//Disabling backface culling
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);

	//Clear
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}
