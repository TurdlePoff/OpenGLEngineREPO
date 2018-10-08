#pragma once
/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "Terrain.h"
* Description	: Terrain Declaration file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "Utility.h"

class Terrain
{
public:
	//Inline functions
	inline float GetPercentage(float value, const float min, const float max); //returns ratio of value between min and max
	inline int GetFileLength(std::istream& file); //Get num characters/length of file

	//Functions for creating and deleting buffers
	inline void CreateVertexBuffer(GLuint& vboID);
	inline void DeleteVertexBuffer(GLuint& vboID);
	inline void CreateTexture(GLuint& texID);
	inline void DeleteTexture(GLuint& texID);


	//Main methods
	Terrain(float heightScale = 500.0f, float blockScale = 2.0f);
	virtual ~Terrain();

	void Terminate();	//Deallocation and deletion 
	bool LoadHeightmap(const std::string& filename, unsigned char bitsPerPixel, unsigned int width, unsigned int height);
	bool LoadTexture(const std::string& filename, unsigned int textureStage = 0);


	//Gets the height of the terrain at a location of a point in the world
	float GetHeightAt(const glm::vec3& position);

	//Renders the terrain
	void Render();

	//Renders the normals of the terrain in debug mode.
	void DebugRender();


	
	inline float GetHeightValue(const unsigned char* data, unsigned char numBytes);

protected:
	// Generates the vertex buffer objects from the
	// position, normal, texture, and color buffers
	void GenerateVertexBuffers();
	void GenerateIndexBuffer();

	void GenerateNormals();
	void RenderNormals();

private:
	typedef std::vector<glm::vec3>  PositionBuffer;
	typedef std::vector<glm::vec4>  ColorBuffer;
	typedef std::vector<glm::vec3>  NormalBuffer;
	typedef std::vector<glm::vec2>  TexCoordBuffer;
	typedef std::vector<GLuint>     IndexBuffer;

	PositionBuffer  m_PositionBuffer;
	ColorBuffer     m_ColorBuffer;
	NormalBuffer    m_NormalBuffer;
	TexCoordBuffer  m_Tex0Buffer;
	IndexBuffer     m_IndexBuffer;

	// ID's for the VBO's
	GLuint m_GLVertexBuffer;
	GLuint m_GLNormalBuffer;
	GLuint m_GLColorBuffer;
	GLuint m_GLTex0Buffer;
	GLuint m_GLTex1Buffer;
	GLuint m_GLTex2Buffer;
	GLuint m_GLIndexBuffer;

	static const unsigned int m_uiNumTextures = 3;
	GLuint m_GLTextures[m_uiNumTextures];

	GLuint m_program;

	glm::mat4x4 m_LocalToWorldMatrix;
	glm::mat4x4 m_InverseLocalToWorldMatrix;

	// The dimensions of the heightmap texture
	glm::vec2 m_HeightmapDimensions;

	// The height-map value will be multiplied by this value
	// before it is assigned to the vertex's Y-coordinate
	float m_fHeightScale;

	// The vertex's X and Z coordinates will be multiplied by this
	// for each step when building the terrain
	float m_fBlockScale;
};