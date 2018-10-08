/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "Entity.cpp"
* Description	: Entity implementation file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "Entity.h"
#include "ShaderLoader.h"
#include "Camera.h"

Entity::Entity()
{
	
}

Entity::~Entity(){}

/***********************
* Init: Initialises the entity object
* @author: Vivian Ngo
* @date: 08/05/18
* @parameter: _imageType
***********************/
void Entity::Init(const char* _imageType, EShapes _shape)
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
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
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

/***********************
* GetTextureFile: Gets texture file location
* @author: Vivian Ngo
* @date: 08/05/18
* @return: a file location for the specified sprite
***********************/
//const char* Entity::GetTextureFile(EImage _spriteType)
//{
//	if (_spriteType == ROUNDBIRB)
//	{
//		return SPR_BIRD;
//	}
//	else if (_spriteType == FASTBIRB)
//	{
//		return SPR_TRIBIRD;
//	}
//	else if (_spriteType == PIG)
//	{
//		return SPR_PIG;
//	}
//	else if (_spriteType == FLOOR)
//	{
//		return SPR_FLOOR;
//	}
//	else if (_spriteType == BACKGROUND)
//	{
//		return SPR_BACKGROUND;
//	}
//	return "";	//if NONE
//}

void Entity::Process(float _deltaTick)
{
	
}

/***********************
* Render: Renders entity
* @author: Vivian Ngo
* @date: 08/05/18
***********************/
void Entity::Render()
{
	glUseProgram(m_program);
	glBindVertexArray(m_vao);

	

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

/***********************
* Translate: Translate the sprite
* @author: Vivian Ngo
* @date: 08/05/18
* @parameter: Movement - move sprite
***********************/
void Entity::SetPos(glm::vec3 Movement)
{
	m_vPos = Movement;
}

/***********************
* SetRotatation: SetRotatation of sprite
* @author: Vivian Ngo
* @date: 08/05/18
* @parameter: Rotation - new rotation of sprite
***********************/
void Entity::SetRotatation(glm::vec3 Rotation)
{
	m_vRot = Rotation;
}

/***********************
* SetScale: Set Scale of the sprite
* @author: Vivian Ngo
* @date: 08/05/18
* @parameter: scale - scale sprite
***********************/
void Entity::SetScale(glm::vec3 scale)
{
	m_vScale = scale;
}

/***********************
* GetPos: Get Pos of the sprite
* @author: Vivian Ngo
* @date: 08/05/18
* @return: m_vPos - Get pos of sprite
***********************/
glm::vec3 Entity::GetPos()
{
	return m_vPos;
}

/***********************
* GetRot: Get rotation of the sprite
* @author: Vivian Ngo
* @date: 08/05/18
* @return: m_vRotation - Get rotation of sprite
***********************/
glm::vec3 Entity::GetRot()
{
	return m_vRot;
}

/***********************
* GetScale: Get scale of the sprite
* @author: Vivian Ngo
* @date: 08/05/18
* @return: m_vScale - Get scale of sprite
***********************/
glm::vec3 Entity::GetScale()
{
	return m_vScale;
}



/***********************
* InitB2Body: Initialise a box2d body
* @author: Vivian Ngo
* @date: 08/05/18
* @parameter: _world - the world body used to create a new body
* @parameter: _bodyType - determines whether body is static, kinematic or dynamic
* @parameter: _shape - shape of body
* @parameter: _isFixed - is rotation of body fixed
* @parameter: _hasFixture - does body have fixtures
***********************/
//void Entity::InitB2Body(b2World & _world, b2BodyType _bodyType, EShapes _shape, bool _isFixed, bool _hasFixture)
//{
//	//Creating a body definition
//	b2BodyDef bodyDef;
//	//Define which body type, Dynamic, Static or Kinematic
//	bodyDef.type = _bodyType;
//	//Sets the position of the body to the same position as the game entity 
//	bodyDef.position.Set(m_vPos.x, m_vPos.y);
//	//Box2D version of a pointer, which points from a physics object to a game entity
//	bodyDef.userData = &*this;
//	//Create body
//	m_b2body = _world.CreateBody(&bodyDef);
//	//Set the transform of the physics object according to the transform of the game entity
//	m_b2body->SetTransform(bodyDef.position, (m_vRot.z / 180) * b2_pi);
//	//Only sets fix rotation if the passed in value is false(therefore cannot rotate)
//	m_b2body->SetFixedRotation(_isFixed);
//	b2MassData mass = { 20.0f, m_b2body->GetLocalCenter(), 0.0f };
//	m_b2body->SetMassData(&mass);
//	std::cout << m_b2body->GetMass() << std::endl;
//
//	//Setting shape
//	switch (_shape)
//	{
//	case POLYGON:
//	{
//		b2PolygonShape polygonShape;
//		polygonShape.SetAsBox(m_vScale.x / 2, m_vScale.y / 2);
//		if (_hasFixture)
//		{
//			//Define fixture
//			b2FixtureDef fixtureDef;
//			fixtureDef.shape = &polygonShape;
//			//Setting the density
//			fixtureDef.density = Utils::DENSITY;
//			//Setting the friction
//			fixtureDef.friction = Utils::FRICTION;
//			//add this fixture to the body
//			m_b2body->CreateFixture(&fixtureDef);
//		}
//		else
//		{
//			m_b2body->CreateFixture(&polygonShape, 0.0f);
//		}
//		break;
//	}
//	case CIRCLE:
//	{
//		b2CircleShape b2Circle;
//		b2Circle.m_radius = static_cast<float32>(m_vScale.x / 2);
//		if (_hasFixture)
//		{
//			//Define fixture
//			b2FixtureDef fixtureDef;
//			fixtureDef.shape = &b2Circle;
//			//Setting the density
//			fixtureDef.density = Utils::DENSITY;
//			//Setting the friction
//			fixtureDef.friction = Utils::FRICTION;
//			//add this fixture to the body
//			m_b2body->CreateFixture(&fixtureDef);
//		}
//		else
//		{
//			m_b2body->CreateFixture(&b2Circle, 0.0f);
//		}
//		break;
//	}
//	}
//}
