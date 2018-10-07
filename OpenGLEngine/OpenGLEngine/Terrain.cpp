#pragma once
/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name		: "Terrain.cpp"
* Description	: Terrain implementation file. 
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

// Macros for generating the terrain

// Enable mutitexture blending across the terrain
// If set to 0, the texture in stage 0 will be used for the entire terrain
#ifndef ENABLE_MULTITEXTURE
#define ENABLE_MULTITEXTURE 1
#endif

// Enable the blend constants based on the slope of the terrain
// If slope-based blending is enabled, textures in 0 and 1 will be blended based on the slope
// If slope-based blending is diabled (set to 0), textures will be blended based on the height 
//																  of the terrain at that point
#ifndef ENABLE_SLOPE_BASED_BLEND
#define ENABLE_SLOPE_BASED_BLEND 1
#endif 

// Prevents compiler warnings of conversions from ints to pointers whebn binging vbos
#define BUFFER_OFFSET(i) ((char*)NULL + (i))

#include "Terrain.h"
#include "ShaderLoader.h"
#include "Camera.h"


/***********************
* GetPercentage: Inline math function that gets the ratio of a value between min and max.
* @parameter: value - value to check
* @parameter: min - the minimum value to check
* @parameter: max - the maximum value to check
* @author: Vivian Ngo
* @date: 04 / 10 / 18
* @return: If val <= min, return 0 else return 1
***********************/
inline float Terrain::GetPercentage(float value, const float min, const float max)
{
	value = glm::clamp(value, min, max);
	return (value - min) / (max - min);
}

/***********************
* GetFileLength: Inline function gets the length of a file
* @parameter: file - file to check
* @author: Vivian Ngo
* @date: 04 / 10 / 18
* @return: If val <= min, return 0 else return 1
***********************/
inline int Terrain::GetFileLength(std::istream & file)
{
	// Find the last character in the file given
	int pos = static_cast<int>(file.tellg());
	file.seekg(0, std::ios::end);
	int length = static_cast<int>(file.tellg());
	file.seekg(pos);

	return length;
}

/***********************
* CreateVertexBuffer: Inline function that creates a vertex buffer for the given ID
* @parameter: vboID - ID of the vertex buffer to create and set
* @author: Vivian Ngo
* @date: 04 / 10 / 18
***********************/
inline void Terrain::CreateVertexBuffer(GLuint & vboID)
{
	// Make sure we don't lose the reference to the previous VBO if there is one
	DeleteVertexBuffer(vboID);
	glGenBuffersARB(1, &vboID);
}

/***********************
* DeleteVertexBuffer: Inline function that deletes a vertex buffer for the given ID
* @parameter: vboID - ID of the vertex buffer to create and set
* @author: Vivian Ngo
* @date: 04 / 10 / 18
***********************/
inline void Terrain::DeleteVertexBuffer(GLuint & vboID)
{
	if (vboID != 0)
	{
		glDeleteBuffersARB(1, &vboID);
		vboID = 0;
	}
}

/***********************
* CreateTexture: Inline function that creates a texture buffer for the given ID
* @parameter: texID - ID of the texture buffer to create and set
* @author: Vivian Ngo
* @date: 04 / 10 / 18
***********************/
inline void Terrain::CreateTexture(GLuint & texID)
{
	DeleteTexture(texID);
	glGenTextures(1, &texID);
}

/***********************
* DeleteTexture: Inline function that deletes a texture buffer for the given ID
* @parameter: texID - ID of the texture buffer to delete
* @author: Vivian Ngo
* @date: 04 / 10 / 18
***********************/
inline void Terrain::DeleteTexture(GLuint & texID)
{
	if (texID != 0)
	{
		glDeleteTextures(1, &texID);
		texID = 0;
	}
}

/***********************
* GetHeightValue: Inline function that translates the given char-data-array into a float value
* @parameter: data - char-data-array to convert
* @parameter: numBytes - bytes to convert (1 = 8-bit, 2bytes = 16-bits, 4bytes = 32-bit)
* @author: Vivian Ngo
* @date: 04 / 10 / 18
***********************/
inline float Terrain::GetHeightValue(const unsigned char * data, unsigned char numBytes)
{
	switch (numBytes)
	{
	case 1:
	{
		return (unsigned char)(data[0]) / (float)0xff;
	}
	break;
	case 2:
	{
		return (unsigned short)(data[1] << 8 | data[0]) / (float)0xffff;
	}
	break;
	case 4:
	{
		return (unsigned int)(data[3] << 24 | data[2] << 16 | data[1] << 8 | data[0]) / (float)0xffffffff;
	}
	break;
	default:
	{
		assert(false);  // Height field with non standard pixel sizes
	}
	break;
	}

	return 0.0f;
}


/***********************
* Terrain Constructor: Set up scene items
* @parameter: heightScale - Max height of terrain 
* @parameter: blockScale - Space between terrain x and y vertices
* @author: Vivian Ngo
* @date: 04 / 10 / 18
***********************/
Terrain::Terrain(float heightScale, float blockScale)
	: m_GLVertexBuffer(0)
	, m_GLNormalBuffer(0)
	, m_GLColorBuffer(0)
	, m_GLTex0Buffer(0)
	, m_GLTex1Buffer(0)
	, m_GLTex2Buffer(0)
	, m_GLIndexBuffer(0)
	, m_LocalToWorldMatrix(1)
	, m_InverseLocalToWorldMatrix(1)
	, m_HeightmapDimensions(0, 0)
	, m_fHeightScale(heightScale)
	, m_fBlockScale(blockScale)
{
	//Initialise all slots in m_GLTextures array with 0
	memset(m_GLTextures, 0, sizeof(m_GLTextures));


	if (!LoadHeightmap("Resources/Terrain/terrain0-16bbp-257x257.raw", 16, 257, 257))
	{
		std::cerr << "Failed to load heightmap for terrain!" << std::endl;
	}
	if (!LoadTexture("Resources/Terrain/grass.png", 0))
	{
		std::cerr << "Failed to load terrain texture for texture stage 0!" << std::endl;
	}
	if (!LoadTexture("Resources/Terrain/Mountain Faults.jpg", 1))
	{
		std::cerr << "Failed to load terrain texture for texture stage 1!" << std::endl;
	}
	if (!LoadTexture("Resources/Terrain/Ice Age.jpg", 2))
	{
		std::cerr << "Failed to load terrain texture for texture stage 2!" << std::endl;
	}
}

Terrain::~Terrain()
{
	Terminate();
}

/***********************
* Terminate: Deallocates all allocated memory and deletes textures and vbos stored in gpu
* @parameter: Max height of terrain
* @parameter: Space between terrain x and y vertices
* @author: Vivian Ngo
* @date: 04 / 10 / 18
***********************/
void Terrain::Terminate()
{
	DeleteVertexBuffer(m_GLVertexBuffer);
	DeleteVertexBuffer(m_GLNormalBuffer);
	DeleteVertexBuffer(m_GLColorBuffer);
	DeleteVertexBuffer(m_GLTex0Buffer);
	DeleteVertexBuffer(m_GLTex1Buffer);
	DeleteVertexBuffer(m_GLTex2Buffer);
	DeleteVertexBuffer(m_GLIndexBuffer);

	for (unsigned int i = 0; i < m_uiNumTextures; ++i)
	{
		DeleteTexture(m_GLTextures[i]);
	}
}

/***********************
* LoadHeightmap: Loads heightmap from raw file
* @parameter: filename - name of the raw file
* @parameter: bitsPerPixel - bits per pixel of heightmap
* @parameter: width - px width of height map texture
* @parameter: height - px height of height map texture
* @author: Vivian Ngo
* @date: 04 / 10 / 18
* @return: returns true if heightmap is successfully loaded
***********************/
bool Terrain::LoadHeightmap(const std::string & filename, unsigned char bitsPerPixel, unsigned int width, unsigned int height)
{
	std::ifstream inFile;

	//If file doesnt exist, loading has failed.
	if (!std::experimental::filesystem::exists(filename))
	{
		std::cerr << "Could not find file: " << filename << std::endl;
		return false;
	}

	//If file doesnt open, loading has failed.
	inFile.open(filename, std::ios_base::binary);
	if (inFile.fail())
	{
		std::cerr << "Failed to open file: " << filename << std::endl;
		return false;
	}

	//Verify the file is the size we are expecting based on the passed-in parameters
	const unsigned int bytesPerPixel = bitsPerPixel / 8;
	const unsigned int expectedFileSize = (bytesPerPixel * width * height);
	const unsigned int fileSize = GetFileLength(inFile);

	if (expectedFileSize != fileSize)
	{
		std::cerr << "Expected file size [" << expectedFileSize 
				  << " bytes] differs from actual file size [" << fileSize << " bytes]" << std::endl;
		return false;
	}

	//Load the height map data from the raw file into an unsigned char array
	unsigned char* heightMap = new unsigned char[fileSize];
	inFile.read((char*)heightMap, fileSize);

	//If an error occurs when reading from the file, loading has failed.
	if (inFile.fail())
	{
		std::cerr << "An error occurred while reading from the height map file: " << filename << std::endl;
		inFile.close();
		delete[] heightMap;
		return false;
	}

	inFile.close(); //Close the file 

	//Start building vertex buffer--------------------
	unsigned int numVerts = width * height;
	m_PositionBuffer.resize(numVerts);
	m_ColorBuffer.resize(numVerts);
	m_NormalBuffer.resize(numVerts);
	m_Tex0Buffer.resize(numVerts);

	m_HeightmapDimensions = glm::vec2(width, height);

	//Size of terrain in world units
	float terrainWidth = (width - 1) * m_fBlockScale;
	float terrainHeight = (height - 1) * m_fBlockScale;

	//To centre terrain around local origin
	float halfTerrainWidth = terrainWidth * 0.5f;	//x axis
	float halfTerrainHeight = terrainHeight * 0.5f;	//z axis

	//For each pixel of the heightmap, compute their vertex positions and texture coordinates
	for (unsigned int j = 0; j < height; ++j)
	{
		for (unsigned int i = 0; i < width; ++i)
		{
			unsigned int index = (j * width) + i;
			assert(index * bytesPerPixel < fileSize);
			float heightValue = GetHeightValue(&heightMap[index * bytesPerPixel], bytesPerPixel);

			float S = (i / (float)(width - 1));
			float T = (j / (float)(height - 1));

			float X = (S * terrainWidth) - halfTerrainWidth;
			float Y = heightValue * m_fHeightScale;
			float Z = (T * terrainHeight) - halfTerrainHeight;

			// Blend 3 textures depending on the height of the terrain
			float tex0Contribution = 1.0f - GetPercentage(heightValue, 0.0f, 0.75f);
			float tex2Contribution = 1.0f - GetPercentage(heightValue, 0.75f, 1.0f);

			m_NormalBuffer[index] = glm::vec3(0);
			m_PositionBuffer[index] = glm::vec3(X, Y, Z);

			#if ENABLE_MULTITEXTURE
				m_ColorBuffer[index] = glm::vec4(tex0Contribution, tex0Contribution, tex0Contribution, tex2Contribution);
			#else
				m_ColorBuffer[index] = glm::vec4(1.0f);
			#endif
				m_Tex0Buffer[index] = glm::vec2(S, T);
		}
	}

	std::cout << "Terrain Successfully Loaded!" << std::endl;
	delete[] heightMap;

	//Generate buffers and normals
	GenerateIndexBuffer();
	GenerateNormals();
	GenerateVertexBuffers();

	return true;
}

/***********************
* LoadTexture: Loads texture which will be blended with other textures
* @parameter: filename - name of the texture file
* @parameter: textureStage - (0,1,2) 
*				0 - lowest points of terrain, 
*				1 - stage between high and low points
*				2 - highest points of terrain, 
* @author: Vivian Ngo
* @date: 04 / 10 / 18
* @return: returns true if texture is successfully loaded
***********************/
bool Terrain::LoadTexture(const std::string & filename, unsigned int textureStage)
{
	assert(textureStage < m_uiNumTextures); //texture stage should be in the range of 0-2
	DeleteTexture(m_GLTextures[textureStage]); //If texture at this location happens to already exist for whatever reason, delete.

	//Load texture into array
	m_GLTextures[textureStage] = SOIL_load_OGL_texture(filename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	//Bind texture only if texture was properly loaded
	if (m_GLTextures[textureStage] != 0)
	{
		glBindTexture(GL_TEXTURE_2D, m_GLTextures[textureStage]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, 0);
		return true;
	}
	else
	{
		return false;
	}
}

/***********************
* GetHeightAt: Gets the height of the terrain at a location of a point in the world
* @parameter: position - world-space position to get the height for
* @author: Vivian Ngo
* @date: 04 / 10 / 18
* @return: returns the height of the position given.
*		   However, if the position given is not part of the terrain, 
*		   a large negative number will be returned.
***********************/
float Terrain::GetHeightAt(const glm::vec3 & position)
{
	// Set default value as a very large negative number (which is returned if player is not on terrain)
	float height = -FLT_MAX;

	// Check if the terrain dimensions are valid
	if (m_HeightmapDimensions.x < 2 || m_HeightmapDimensions.y < 2) return height;

	// Width and height of the terrain in world units
	float terrainWidth = (m_HeightmapDimensions.x - 1) * m_fBlockScale;
	float terrainHeight = (m_HeightmapDimensions.y - 1) * m_fBlockScale;
	float halfWidth = terrainWidth * 0.5f;
	float halfHeight = terrainHeight * 0.5f;

	// Multiple the position by the inverse of the terrain matrix to get the position in terrain local space
	glm::vec3 terrainPos = glm::vec3(m_InverseLocalToWorldMatrix * glm::vec4(position, 1.0f));
	glm::vec3 invBlockScale(1.0f / m_fBlockScale, 0.0f, 1.0f / m_fBlockScale);

	// Calculate an offset and scale to get the vertex indices
	glm::vec3 offset(halfWidth, 0.0f, halfHeight);

	// Get the 4 vertices that make up the triangle we're over
	glm::vec3 vertexIndices = (terrainPos + offset) * invBlockScale; //rational index of verts in the 2d grid of the mesh


	//If the fractional part of the vertex index in the X-axis is greater 
	//	than the fractional part of the vert index in the z-axis, 
	//	then the object is inside the upper triangle of the quad

	//	NOTE: Floorf- returns biggest float that is less than or = to the given val

	int u0 = (int)floorf(vertexIndices.x); 
	int u1 = u0 + 1;	
	int v0 = (int)floorf(vertexIndices.z);
	int v1 = v0 + 1;

	if (u0 >= 0 && u1 < m_HeightmapDimensions.x && v0 >= 0 && v1 < m_HeightmapDimensions.y)
	{
		// Grab the heights of the cell we are in.
		// A*--*B
		//  |\ |
		//  | \|
		// C*--*D
		glm::vec3 A = m_PositionBuffer[static_cast<int>((v0 * m_HeightmapDimensions.x) + u0)];    // Top-left vertex
		glm::vec3 B = m_PositionBuffer[static_cast<int>((v0 * m_HeightmapDimensions.x) + u1)];    // Top-right vertex
		glm::vec3 C = m_PositionBuffer[static_cast<int>((v1 * m_HeightmapDimensions.x) + u0)];    // Bottom-left vertex
		glm::vec3 D = m_PositionBuffer[static_cast<int>((v1 * m_HeightmapDimensions.x) + u1)];    // Bottom-right vertex

		 //Check hich triangle are we over
		float percentU = vertexIndices.x - u0;
		float percentV = vertexIndices.z - v0;

		glm::vec3 dU, dV;
		if (percentU > percentV)
		{   // Top triangle
			dU = B - A;
			dV = D - B;
		}
		else
		{   // Bottom triangle
			dU = D - C;
			dV = C - A;
		}

		//The exact position is computed based on the position of the 
		//	top-left vertex and the ratio between the vertices.
		glm::vec3 heightPos = A + (dU * percentU) + (dV * percentV);

		// Convert back to world-space by multiplying by the terrain's world matrix
		heightPos = glm::vec3(m_LocalToWorldMatrix * glm::vec4(heightPos, 1));
		height = heightPos.y;
	}

	return height;
}

/***********************
* Render: Renders the terrain.
* @author: Vivian Ngo
* @date: 04 / 10 / 18
***********************/
void Terrain::Render()
{
	//Transform the current model-view matrix by the terrain’s local matrix to world matrix 
	//	so that we can view the terrain geometry in object space.
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(glm::value_ptr(m_LocalToWorldMatrix));

	// Texture Stage 0
	// Simply output texture0 for stage 0.
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glScalef(32.0f, 32.0f, 1.0f); //Tile the texture 32 times on x and z axis

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_GLTextures[0]);

	glClientActiveTextureARB(GL_TEXTURE0_ARB);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_GLTex0Buffer);
	glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));

#if ENABLE_MULTITEXTURE
	// Disable lighting because it changes the color of the vertices that are
	// used for the multitexture blending.
	glDisable(GL_LIGHTING);

	//
	// Texture Stage 1
	//
	// Perform a linear interpolation between the output of stage 0 (i.e texture0) 
	// and texture1 and use the RGB portion of the vertex's color to mix the two. 
	//
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glScalef(32.0f, 32.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_GLTextures[1]);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_INTERPOLATE_ARB);

	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_PREVIOUS_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, GL_SRC_COLOR);

	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB, GL_SRC_COLOR);

	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB_ARB, GL_PRIMARY_COLOR_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB, GL_SRC_COLOR);

	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_GLTex1Buffer);
	glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));

	//
	// Texture Stage 2
	//
	// Perform a linear interpolation between the output of stage 1 
	// (i.e texture0 mixed with texture1) and texture2 and use the ALPHA 
	// portion of the vertex's color to mix the two. 
	//
	glActiveTextureARB(GL_TEXTURE2_ARB);
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glScalef(32.0f, 32.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_GLTextures[2]);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_INTERPOLATE_ARB);

	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_PREVIOUS_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, GL_SRC_COLOR);

	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB, GL_SRC_COLOR);

	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB_ARB, GL_PRIMARY_COLOR_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB, GL_SRC_ALPHA);

	glClientActiveTextureARB(GL_TEXTURE2_ARB);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_GLTex2Buffer);
	glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));

#else
	//If we are not using the multi - texture technique, 
	//	enable texture and lighting on the terrain to achieve “shadows” 
	//	on terrain polygons that are facing away from the light
	glEnable(GL_TEXTURE);
	glEnable(GL_LIGHTING);
#endif

	//Bind all other vbos for the vertex position, color, and normal.

	//Enable all client states (default disabled)
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	//Bind vbos
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_GLVertexBuffer);
	glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_GLColorBuffer);
	glColorPointer(4, GL_FLOAT, 0, BUFFER_OFFSET(0));
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_GLNormalBuffer);
	glNormalPointer(GL_FLOAT, 0, BUFFER_OFFSET(0));

	//Bind ibo
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_GLIndexBuffer);

	//Draw the terrain yAYYY
	glDrawElements(GL_TRIANGLES, m_IndexBuffer.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	//Return all the states to normal so other draw calls don't screw up

	//Disable all client states
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

#if ENABLE_MULTITEXTURE
	glActiveTextureARB(GL_TEXTURE2_ARB);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glClientActiveTextureARB(GL_TEXTURE2_ARB);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#endif

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glClientActiveTextureARB(GL_TEXTURE0_ARB);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

#if _DEBUG
	DebugRender();
#endif

}

/***********************
* DebugRender: Renders the normals of the terrain in debug mode.
* @author: Vivian Ngo
* @date: 04 / 10 / 18
***********************/
void Terrain::DebugRender()
{
	RenderNormals();
}

/***********************
* GenerateVertexBuffers: Generates the vbos needed for the terrain
* @author: Vivian Ngo
* @date: 04 / 10 / 18
***********************/
void Terrain::GenerateVertexBuffers()
{
	//Generate the buffer object ID's
	CreateVertexBuffer(m_GLVertexBuffer);
	CreateVertexBuffer(m_GLNormalBuffer);
	CreateVertexBuffer(m_GLColorBuffer);
	CreateVertexBuffer(m_GLTex0Buffer);
	CreateVertexBuffer(m_GLTex1Buffer);
	CreateVertexBuffer(m_GLTex2Buffer);
	CreateVertexBuffer(m_GLIndexBuffer);

	//Bind all the buffer objects
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_GLVertexBuffer);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(glm::vec3) * m_PositionBuffer.size(), &(m_PositionBuffer[0]), GL_STATIC_DRAW_ARB);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_GLColorBuffer);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(glm::vec4) * m_ColorBuffer.size(), &(m_ColorBuffer[0]), GL_STATIC_DRAW_ARB);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_GLNormalBuffer);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(glm::vec3) * m_NormalBuffer.size(), &(m_NormalBuffer[0]), GL_STATIC_DRAW_ARB);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_GLTex0Buffer);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(glm::vec2) * m_Tex0Buffer.size(), &(m_Tex0Buffer[0]), GL_STATIC_DRAW_ARB);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_GLTex1Buffer);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(glm::vec2) * m_Tex0Buffer.size(), &(m_Tex0Buffer[0]), GL_STATIC_DRAW_ARB);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_GLTex2Buffer);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(glm::vec2) * m_Tex0Buffer.size(), &(m_Tex0Buffer[0]), GL_STATIC_DRAW_ARB);

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_GLIndexBuffer);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(GLuint) * m_IndexBuffer.size(), &(m_IndexBuffer[0]), GL_STATIC_DRAW_ARB);

	glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
}

/***********************
* GenerateIndexBuffer: Generates the ibos needed for the terrain
* @author: Vivian Ngo
* @date: 04 / 10 / 18
***********************/
void Terrain::GenerateIndexBuffer()
{
	//Dont generate if terrain hasn't been loaded properly, or if it isnt a correct size
	if (m_HeightmapDimensions.x < 2 || m_HeightmapDimensions.y < 2)
	{
		return;
	}

	const unsigned int terrainWidth = static_cast<const unsigned int>(m_HeightmapDimensions.x);
	const unsigned int terrainHeight = static_cast<const unsigned int>(m_HeightmapDimensions.y);

	// 2 triangles for every quad of the terrain mesh
	const unsigned int numTriangles = (terrainWidth - 1) * (terrainHeight - 1) * 2;

	// 3 indices for each triangle in the terrain mesh
	m_IndexBuffer.resize(numTriangles * 3);

	//Create indices for the index buffer
	unsigned int index = 0; // Index in the index buffer
	for (unsigned int j = 0; j < (terrainHeight - 1); ++j)
	{
		for (unsigned int i = 0; i < (terrainWidth - 1); ++i)
		{
			int vertexIndex = (j * terrainWidth) + i;

			// Top triangle (T0)
			m_IndexBuffer[index++] = vertexIndex;                           // V0
			m_IndexBuffer[index++] = vertexIndex + terrainWidth + 1;        // V3
			m_IndexBuffer[index++] = vertexIndex + 1;                       // V1
			
			// Bottom triangle (T1)
			m_IndexBuffer[index++] = vertexIndex;                           // V0
			m_IndexBuffer[index++] = vertexIndex + terrainWidth;            // V2
			m_IndexBuffer[index++] = vertexIndex + terrainWidth + 1;        // V3
		}
	}
}

/***********************
* GenerateNormals: Generates the normals needed for the terrain
* @author: Vivian Ngo
* @date: 04 / 10 / 18
***********************/
void Terrain::GenerateNormals()
{	
	//Compute normals of each triangle in the index buffer
	for (unsigned int i = 0; i < m_IndexBuffer.size(); i += 3)
	{
		glm::vec3 v0 = m_PositionBuffer[m_IndexBuffer[i + 0]];
		glm::vec3 v1 = m_PositionBuffer[m_IndexBuffer[i + 1]];
		glm::vec3 v2 = m_PositionBuffer[m_IndexBuffer[i + 2]];

		//Compute normal by getting the cross product of two of the triangle edges
		glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

		m_NormalBuffer[m_IndexBuffer[i + 0]] += normal;
		m_NormalBuffer[m_IndexBuffer[i + 1]] += normal;
		m_NormalBuffer[m_IndexBuffer[i + 2]] += normal;
	}

	//normalize the summed normals in order to determine the slope of the vertex at a point
	const glm::vec3 UP(0.0f, 1.0f, 0.0f); //Up vector
	for (unsigned int i = 0; i < m_NormalBuffer.size(); ++i)
	{
		m_NormalBuffer[i] = glm::normalize(m_NormalBuffer[i]);

		//Compute blending factor between different textures on the terrain
		//Blending is done to make the terrain look more realistic and less stretched //TODO CHECK
		//To disable slope-based blending, set ENABLE_SLOPE_BASED_BLEND to 0
#if ENABLE_SLOPE_BASED_BLEND
		float fTexture0Contribution = glm::clamp(glm::dot(m_NormalBuffer[i], UP) - 0.1f, 0.0f, 1.0f);
		m_ColorBuffer[i] = glm::vec4(fTexture0Contribution, fTexture0Contribution, fTexture0Contribution, m_ColorBuffer[i].w);
#endif
	}
}

/***********************
* RenderNormals: Renders the normals for the terrain
* @author: Vivian Ngo
* @date: 04 / 10 / 18
***********************/
void Terrain::RenderNormals()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(glm::value_ptr(m_LocalToWorldMatrix));

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);

	glColor3f(1.0f, 1.0f, 0.0f);// Yellow

	glBegin(GL_LINES);
	{
		for (unsigned int i = 0; i < m_PositionBuffer.size(); ++i)
		{
			glm::vec3 p0 = m_PositionBuffer[i];
			glm::vec3 p1 = (m_PositionBuffer[i] + m_NormalBuffer[i]);

			glVertex3fv(glm::value_ptr(p0));
			glVertex3fv(glm::value_ptr(p1));
		}
	}
	glEnd();

	glPopAttrib();

	glPopMatrix();
}
