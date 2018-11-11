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

	glGenVertexArrays(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vecPosition.size(), &m_vecPosition[0],	GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
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