/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "Cloth.h"
* Description	: Cloth Implementation file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/
#include "Cloth.h"
#include "Input.h"

Cloth::Cloth()
{
	m_program = ShaderLoader::GetInstance()->GetProgram((char*)"Plain");

	m_vPos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_vRot = glm::vec3();
	m_vScale = glm::vec3(1.0f, 1.0f, 1.0f);
}

/***********************
* Cloth: Destructor
* @author: Vivian Ngo
***********************/
Cloth::~Cloth()
{
}

/***********************
* Init: Initialises the cloth
* @author: Vivian Ngo
* @parameter: _width - width of cloth
* @parameter: _height - height of cloth
* @parameter: _numParticlesWidth - num of particles in width
* @parameter: _numParticlesHeight - num of particles in height
***********************/
void Cloth::Init(float _width, float _height, int _numParticlesWidth, int _numParticlesHeight, glm::vec3 _pos)
{
	m_fWidth = _width;
	m_fHeight = _height;
	//Reset particles and constraints
	m_fParticlesWidth = _numParticlesWidth;
	m_fParticlesHeight = _numParticlesHeight;

	m_vParticles.clear();
	m_vConstraints.clear();

	m_vParticles.resize(_numParticlesWidth * _numParticlesHeight); 
	
	int verticesID = 0;
	// creating particles in a grid of particles from (0,0,0) to (width,-height,0)
	for (int y = 0; y<_numParticlesWidth; y++)
	{
		for (int x = 0; x<_numParticlesHeight; x++)
		{ 
			float xOffset = 0.0f;
			
			//Give x particles an offsetted x position .
			//so it doesnt start off as a flat cloth
			if (x % 2 == 0)
			{
				xOffset = -0.2f;
			}
			else
			{
				xOffset = 0.2f;
			}

			glm::vec3 pos = glm::vec3(_width * (x / (float)_numParticlesWidth) + _pos.x,
				-_height * (y / (float)_numParticlesHeight) + _pos.y,
				 _pos.z + xOffset);
			m_vParticles[y*_numParticlesWidth + x] = ClothParticle(pos); // insert particle in column x at y'th row

			//Set vertices id and vertices for each particle
			m_vParticles[y*_numParticlesWidth + x].SetVertID(verticesID);
			++verticesID;

			//Position values
			m_fVerticesPoints.push_back(pos.x);
			m_fVerticesPoints.push_back(pos.y);
			m_fVerticesPoints.push_back(pos.z);

			//Normal values
			m_fVerticesPoints.push_back(0.0f);
			m_fVerticesPoints.push_back(1.0f);
			m_fVerticesPoints.push_back(1.0f);
		}
	}

	// Make constraints for each square
	for (int x = 0; x<_numParticlesWidth; x++)
	{
		for (int y = 0; y<_numParticlesHeight; y++)
		{
			if (x < _numParticlesWidth - 1)
			{
				MakeConstraint(GetParticle(x, y), GetParticle(x + 1, y));
			}
			if (y < _numParticlesHeight - 1)
			{
				MakeConstraint(GetParticle(x, y), GetParticle(x, y + 1));
			}
			if (x < _numParticlesWidth - 1 && y < _numParticlesHeight - 1)
			{
				MakeConstraint(GetParticle(x, y), GetParticle(x + 1, y + 1));
			}
			if (x < _numParticlesWidth - 1 && y < _numParticlesHeight - 1)
			{
				MakeConstraint(GetParticle(x + 1, y), GetParticle(x, y + 1));
			}
		}
	}

	// Set pins at top of the cloth
	float offsetX = -1;
	for (int i = 0; i < _numParticlesWidth; i++)
	{
		GetParticle(0 + i, 0)->SetPinned(true);
	}
	GenerateBuffers();
}

/***********************
* GenerateBuffers: Generate Buffers for the cloth
* @author: Vivian Ngo
***********************/
void Cloth::GenerateBuffers()
{
	m_iIndicesPoints.clear();
	for (auto& constraint : m_vConstraints)
	{
		if (!constraint.GetDestroyed() || constraint.GetParticleTwo()-constraint.GetParticleOne() < 5.0f)
		{
			m_iIndicesPoints.push_back(constraint.GetParticleOne()->GetVertID());
			m_iIndicesPoints.push_back(constraint.GetParticleTwo()->GetVertID());
		}
	}

	//Generating Buffers and Arrays
	glGenVertexArrays(1, &m_vao);					//Vert Array
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);                        //Vert Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glGenBuffers(1, &m_ebo);						//Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

	glBufferData(GL_ARRAY_BUFFER, m_fVerticesPoints.size() * sizeof(GLfloat), &m_fVerticesPoints[0], GL_DYNAMIC_DRAW);			//VBO Buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_iIndicesPoints.size() * sizeof(GLuint), &m_iIndicesPoints[0], GL_DYNAMIC_DRAW);    //EBO Buffer
	m_iIndicesCount = m_iIndicesPoints.size();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}

/***********************
* Render: Renders the cloth
* @author: Vivian Ngo
***********************/
void Cloth::Render()
{
	glUseProgram(m_program);
	glDisable(GL_CULL_FACE);

	//ModelMatrix
	glm::mat4 translation = glm::translate(glm::mat4(), m_vPos);
	glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 rotation = glm::rotate(glm::mat4(), glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
	rotation = glm::rotate(glm::mat4(), glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotation = glm::rotate(glm::mat4(), glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scale = glm::scale(glm::mat4(), m_vScale);

	glm::mat4 Model = translation * rotation * scale;

	glm::mat4 VP = Camera::GetInstance()->GetProjection() * Camera::GetInstance()->GetView();

	//Constantly update vertices and indices when rendering
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo); 

	glBufferSubData(GL_ARRAY_BUFFER, 0, m_fVerticesPoints.size() * sizeof(GLfloat), m_fVerticesPoints.data());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_iIndicesPoints.size() * sizeof(GLuint), &m_iIndicesPoints[0], GL_DYNAMIC_DRAW);
	m_iIndicesCount = m_iIndicesPoints.size();

	glUniformMatrix4fv(glGetUniformLocation(m_program, "MVP"), 1, GL_FALSE, glm::value_ptr(VP * Model));
	
	//Draw the cloth
	glDrawElements(GL_LINES, m_iIndicesCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/***********************
* Process: Processes the cloth
* @author: Vivian Ngo
* @parameter: _deltaTick
***********************/
void Cloth::Process(float _deltaTick)
{
	m_iIndicesPoints.clear();
	for (unsigned int i = 0; i < m_vConstraints.size(); ++i)
	{
		if (!m_vConstraints[i].GetDestroyed())
		{
			m_iIndicesPoints.push_back(m_vConstraints[i].GetParticleOne()->GetVertID());
			m_iIndicesPoints.push_back(m_vConstraints[i].GetParticleTwo()->GetVertID());
		}
	}

	std::vector<Constraint>::iterator constraint;
	for (int i = 0; i < CONSTRAINT_ITERATIONS; i++) // iterate over all constraints several times
	{
		for (constraint = m_vConstraints.begin(); constraint != m_vConstraints.end(); constraint++)
		{
			if (!(*constraint).GetDestroyed())
			{
				(*constraint).Process(_deltaTick); // satisfy constraint.
			}
		}
	}

	int i = 0;
	std::vector<ClothParticle>::iterator particle;
	for (particle = m_vParticles.begin(); particle != m_vParticles.end(); particle++)
	{
		//Process the particle
		(*particle).Process(_deltaTick); 

		//Update the positions of the particles
		m_fVerticesPoints[i] = (particle->GetPos().x);
		m_fVerticesPoints[i + 1] = (particle->GetPos().y);
		m_fVerticesPoints[i + 2] = (particle->GetPos().z);
		i += 6;

		//Handle particle picking if left mouse clicked and dragged
		if (Input::GetInstance()->MouseState[0] == INPUT_HOLD)
		{
			if (!m_isHoldingParticle)
			{
				ProcessParticlePick(&(*particle));
			}
			else
			{
				ProcessParticlePick(pickedParticle);
			}
		}
		else
		{
			//set as not holding a particle
			m_isHoldingParticle = false;
		}
	}

}

/***********************
* AddForce: Adds Force the cloth
* @author: Vivian Ngo
* @parameter: _deltaTick
***********************/
void Cloth::AddForce(const glm::vec3 _direction)
{
	std::vector<ClothParticle>::iterator particle;
	for (particle = m_vParticles.begin(); particle != m_vParticles.end(); particle++)
	{
		(*particle).AddForce(_direction); // add the forces to each particle
	}
}

/***********************
* WindForce: Adds World Wind Force to the cloth
* @author: Vivian Ngo
* @parameter: _direction - direction of wind to blow at the cloth
***********************/
void Cloth::WindForce(const glm::vec3 _direction)
{
	for (int x = 0; x < m_fParticlesWidth - 1; ++x)
	{
		for (int y = 0; y<m_fParticlesHeight - 1; ++y)
		{
			AddWindForcesForTriangle(GetParticle(x + 1, y), GetParticle(x, y), GetParticle(x, y + 1), _direction);
			AddWindForcesForTriangle(GetParticle(x + 1, y + 1), GetParticle(x + 1, y), GetParticle(x, y + 1), _direction);
		}
	}
}

/***********************
* BallCollision: Adds Ball Collision to the cloth
* @author: Vivian Ngo
* @parameter: _center - center of ball
* @parameter: _radius - radius of ball
***********************/
void Cloth::BallCollision(const glm::vec3 _center, const float _radius)
{
	float offset = 0.3f;

	//Check collision with ball for each particle
	for (std::vector<ClothParticle>::iterator particle = m_vParticles.begin(); particle != m_vParticles.end(); ++particle)
	{
		glm::vec3 v = (*particle).GetPos() - _center;
		float pDist = glm::distance((*particle).GetPos(), _center);

		//If the particle is inside the ball
		if (pDist < _radius + offset)
		{
			//Adjust the position of the particle so that it is outside of the ball
			(*particle).AdjustPos(glm::normalize(v) * ((_radius + offset) - pDist));
		}
	}
}

/***********************
* UnpinAll: Unpins all pinned particles
* @author: Vivian Ngo
***********************/
void Cloth::UnpinAll()
{
	std::vector<ClothParticle>::iterator particle;
	for (particle = m_vParticles.begin(); particle != m_vParticles.end(); particle++)
	{
		(*particle).SetPinned(false);
	}
}

/***********************
* GetParticle: Gets particle given the position specified
* @author: Vivian Ngo
* @parameter: x - x pos of particle
* @parameter: y - y pos of particle
***********************/
ClothParticle * Cloth::GetParticle(int x, int y)
{
	return &m_vParticles[y * m_fParticlesWidth + x];
}

/***********************
* MakeConstraint: Creates a constraint between two particles (connecion)
* @author: Vivian Ngo
* @parameter: p1 - particle 1 of the constraint
* @parameter: p2 - particle 2 of the constraint
***********************/
void Cloth::MakeConstraint(ClothParticle * p1, ClothParticle * p2)
{
	m_vConstraints.push_back(Constraint(p1, p2));
}

/***********************
* CalcTriangleNormal: Calculates the triangle normal of three particles within the cloth
* @author: Vivian Ngo
* @parameter: p1 - particle 1
* @parameter: p2 - particle 2
* @parameter: p3 - particle 3
***********************/
glm::vec3 Cloth::CalcTriangleNormal(ClothParticle * p1, ClothParticle * p2, ClothParticle * p3)
{
	glm::vec3 pos1 = p1->GetPos();
	glm::vec3 pos2 = p2->GetPos();
	glm::vec3 pos3 = p3->GetPos();

	glm::vec3 v1 = pos2 - pos1;
	glm::vec3 v2 = pos3 - pos1;

	return glm::cross(v1, v2);
}

/***********************
* AddWindForcesForTriangle: Applies wind force to the section applied
* @author: Vivian Ngo
* @parameter: p1 - particle 1
* @parameter: p2 - particle 2
* @parameter: p3 - particle 3
* @parameter: direction - direction of force
***********************/
void Cloth::AddWindForcesForTriangle(ClothParticle * p1, ClothParticle * p2, ClothParticle * p3, const glm::vec3 direction)
{
	glm::vec3 normal = CalcTriangleNormal(p1, p2, p3);
	glm::vec3 d = glm::normalize(normal);
	glm::vec3 force = normal * (glm::dot(d, direction));
	p1->AddForce(force);
	p2->AddForce(force);
	p3->AddForce(force);
}

/***********************
* DeleteRandomParticle: Deletes a Random Particle
* @author: Vivian Ngo
***********************/
void Cloth::DeleteRandomConstraint()
{
	int randIndices = 0;

	randIndices = rand() % (int)m_vConstraints.size();
	m_vConstraints[randIndices].Destroy();
}

/***********************
* ProcessParticlePick: Picks up a particle to move
* @parameter: particle - particle to pick and move
* @author: Vivian Ngo
***********************/
void Cloth::ProcessParticlePick(ClothParticle* particle)
{
	//Calculate mouse click from pointer to world space
	glm::vec3 _position = particle->GetPos();
	float _posRadius = 0.3f;
	glm::vec3 camPos = Camera::GetInstance()->GetCamPos();
	glm::vec3 dirVector = _position - camPos;
	glm::vec3 rayDirection = Input::GetInstance()->ScreenToWorldRay();

	//If player is not already picking a particle
	if (!m_isHoldingParticle)
	{
		//Check if the particle collides with the mouse pointer 
		//based on the radius of the particle
		float fA = glm::dot(rayDirection, rayDirection);
		float fB = 2.0f * glm::dot(dirVector, rayDirection);
		float fC = glm::dot(dirVector, dirVector) - _posRadius * _posRadius;
		float fD = fB * fB - 4.0f * fA * fC;

		//if particle collides with mouse
		if (fD > 0.0f)
		{
			//Set it's initial new position
			float distance = glm::distance(camPos, _position);
			_position = rayDirection * distance + camPos;
			particle->SetPos(_position);

			//Set particle as picked
			pickedParticle = particle;
			m_isHoldingParticle = true;
		}
	}
	else
	{
		//If particle is already being held, calculate new position depending on mouse position
		float distance = glm::distance(camPos, _position);
		_position = rayDirection * distance + camPos;
		particle->SetPos(_position);
		pickedParticle = particle;
	}
}

/***********************
* MoveRings: Moves the rings of the cloth
* @parameter: _squish - Squishes the cloth horizontally when player presses z and spreads it out with x
* @author: Vivian Ngo
***********************/
void Cloth::MoveRings(bool _squish)
{
	//Direction to move rings
	float dir = 1.0f;

	if (!_squish)
	{
		dir = -1.0f;
	}

	//Check each particle in the top row of the cloth
	for(int i = 1; i <= m_fParticlesWidth; ++i)
	{
		int half = m_fParticlesWidth / 2; //Get the value of galf of the particles

		//The speed/amount x to move for particles on the left
		float left = dir * (half / i *0.01f);

		//The speed/amount x to move for particles on the right
		float right = dir * -(half / (m_fParticlesWidth+1 - i) * 0.01f);

		if (i < m_fParticlesWidth/2)
		{
			//if particles are on the left side, move towards the right
			m_vParticles[i-1].AdjustPinnedPos(glm::vec3(left, 0.0f, 0.0f));
		}
		else if (i > m_fParticlesWidth/2)
		{
			//if particles are on the right side, move towards the left
			m_vParticles[i-1].AdjustPinnedPos(glm::vec3(right, 0.0f, 0.0f));
		}
	}
}
