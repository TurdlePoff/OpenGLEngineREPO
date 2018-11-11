/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "ParticleSystem.cpp"
* Description	: ParticleSystem Declaration file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/
#include "ParticleSystem.h"
#include "Camera.h"

/***********************
* ParticleSystem Constructor
* @author: Vivian Ngo
* @parameter: _origin - machine time to process
* @parameter: _textureName - machine time to process
***********************/
ParticleSystem::ParticleSystem(glm::vec3 _origin, const char* _textureName)
{
	m_program = ShaderLoader::GetInstance()->GetProgram((char*)"Fountain");

	m_iParticles = 4000;
	for (int i = 0; i < m_iParticles; i++) 
	{
		m_vecPosition.push_back(glm::vec3()); //initialize position vector
		//m_vecPosition.push_back(glm::vec3(1.0f, 1.0f, 1.0f)); //initialize position vector
		//m_vecPosition.push_back(glm::vec3(0.5f, 0.5f, 0.5f)); //initialize texture vector

		Particle p = Particle(
			_origin, // pos
			glm::vec3(0.25 * cos(i * 0.0167) + 0.25f * Utils::RandomFloat() - 0.125f, // vel
					  2.0f + 0.25f * Utils::RandomFloat() - 0.125f,
					  0.25 * sin(i * 0.0167) + 0.25f * Utils::RandomFloat() - 0.125f
			),
			Utils::RandomFloat() + 0.125f, // elapsed time
			i //particle ID
		); 
		m_vecParticles.push_back(p); // add
	}

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vecPosition.size(), &m_vecPosition[0],	GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);
	////Texture attribute
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(2);

	////Generating and biding the texture
	//glGenTextures(1, &m_texture);
	//glBindTexture(GL_TEXTURE_2D, m_texture);

	////Loading the image
	//unsigned char* image = SOIL_load_image(_textureName, &m_iWidth, &m_iHeight, 0, SOIL_LOAD_RGBA);
	////Defining the texture
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	//glGenerateMipmap(GL_TEXTURE_2D);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	////Generating the texture, freeing up the data and binding it
	//SOIL_free_image_data(image);
	//glBindTexture(GL_TEXTURE_2D, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

/***********************
* ParticleSystem Destructor
* @author: Vivian Ngo
***********************/
ParticleSystem::~ParticleSystem()
{
}

/***********************
* Render: Renders all Particles
* @author: Vivian Ngo
***********************/
void ParticleSystem::Render()
{

	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);

	glm::mat4 translation = glm::translate(glm::mat4(), m_vPos);

	glm::mat4 viewProjection = Camera::GetInstance()->GetView() * Camera::GetInstance()->GetProjection() * translation;
	glm::vec3 vQuad1, vQuad2;
	glm::vec3 vView = Camera::GetInstance()->GetCamFront();
	vView = glm::normalize(vView);
	vQuad1 = glm::cross(vView, Camera::GetInstance()->GetCamUp());
	vQuad1 = glm::normalize(vQuad1);

	vQuad2 = glm::cross(vView, vQuad1);
	vQuad2 = glm::normalize(vQuad2);


	glUseProgram(m_program);
	glBindVertexArray(m_vao);

	glUniform3f(glGetUniformLocation(m_program, "vQuad1"), vQuad1.x, vQuad1.y, vQuad1.z);
	glUniform3f(glGetUniformLocation(m_program, "vQuad2"), vQuad2.x, vQuad2.y, vQuad2.z);

	glUniformMatrix4fv(glGetUniformLocation(m_program, "MVP"), 1, GL_FALSE,
		glm::value_ptr(viewProjection));

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(m_program, "Texture"), 0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vecPosition.size(), &m_vecPosition[0], GL_STATIC_DRAW);
	
	glDrawArrays(GL_POINTS, 0, m_iParticles);
	glBindVertexArray(0);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

/***********************
* Process: Processes all Particles
* @author: Vivian Ngo
* @parameter: _deltaTick - machine time to process
***********************/
void ParticleSystem::Process(float _deltaTick)
{
	for (int i = 0; i < m_iParticles; i++) {
		m_vecParticles[i].Process(_deltaTick);
		m_vecPosition[i] = m_vecParticles[i].GetPos();
	}
	std::sort(m_vecParticles.begin(), m_vecParticles.end(), myComparison);
}

//bool ParticleSystem::myComparison(Particle a, Particle b) {
//	return (a.GetCamDist() > b.GetCamDist());
//}