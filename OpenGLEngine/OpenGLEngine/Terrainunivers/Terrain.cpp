#pragma once

#include "Terrain.h"
#include "Camera.h"
#include "ShaderLoader.h"

Terrain::Terrain()
{
	//m_program = "Resources/Terrain/HeightMap.raw";
	ShaderLoader sLoader;
	m_program = sLoader.CreateProgram((char*)"Resources/Shaders/TextureVertexShader.vs", (char*)"Resources/Shaders/TextureFragmentShader.fs");	
	
	m_info.HeightmapFilename = "Resources/Terrain/HeightMap.raw";
	m_info.HeightScale = 0.35f;
	m_info.HeightOffset = -20.0f;
	m_info.NumRows = 513;
	m_info.NumCols = 513;
	m_info.CellSpacing = 1.0f;
}

void Terrain::InitialiseTerrain()
{
	m_uiNumVertices = m_info.NumRows*m_info.NumCols;
	m_uiNumFaces = (m_info.NumRows - 1)*(m_info.NumCols - 1) * 2;

	vertices.resize(m_uiNumVertices);
	indices.resize(m_uiNumFaces*3);

	LoadHeightmap();
	Smooth();

	BuildVB();
	BuildIB();

	//Generating Buffers and Arrays
	glGenVertexArrays(1, &m_vao);					//Vert Array
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);                        //Vert Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glGenBuffers(1, &m_ebo);						//Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);			//VBO Buffer

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);    //EBO Buffer

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

	int width, height;
	//Loading the image
	unsigned char* image = SOIL_load_image("Resources/Terrain/grass.png", &width, &height, 0, SOIL_LOAD_RGBA);
	//Defining the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
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

void Terrain::RenderTerrain()
{
	glUseProgram(m_program);
	glBindVertexArray(m_vao);

	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glUniform1i(glGetUniformLocation(m_program, "tex"), 0);

	glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f);

	//ModelMatrix
	glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3());
	glm::mat4 rotation = glm::rotate(glm::mat4(), glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
	rotation = glm::rotate(glm::mat4(), glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotation = glm::rotate(glm::mat4(), glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(32.0f, 1.0f, 32.0f));

	glm::mat4 Model = translation * rotation * scale;
	glm::mat4 MVP = Camera::GetInstance()->GetProjection() * Camera::GetInstance()->GetView() * Model;

	glUniformMatrix4fv(glGetUniformLocation(m_program, "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	//Disabling backface culling
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);

	//Clear
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

float Terrain::GetHeight(float x, float z) const
{
	// Transform from terrain local space to "cell" space.
	float c = (x + 0.5f*Width()) / m_info.CellSpacing;
	float d = (z - 0.5f*Depth()) / -m_info.CellSpacing;

	// Get the row and column we are in.
	int row = (int)floorf(d);
	int col = (int)floorf(c);

	// Grab the heights of the cell we are in.
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	float A = m_vHeightmap[row * m_info.NumCols + col];
	float B = m_vHeightmap[row * m_info.NumCols + col + 1];
	float C = m_vHeightmap[(row + 1) * m_info.NumCols + col];
	float D = m_vHeightmap[(row + 1) * m_info.NumCols + col + 1];

	// Where we are relative to the cell.
	float s = c - (float)col;
	float t = d - (float)row;

	// If upper triangle ABC.
	if (s + t <= 1.0f)
	{
		float uy = B - A;
		float vy = C - A;
		return A + s*uy + t*vy;
	}
	else // lower triangle DCB.
	{
		float uy = C - D;
		float vy = B - D;
		return D + (1.0f - s)*uy + (1.0f - t)*vy;
	}
}

float Terrain::Width() const
{
	return (m_info.NumCols - 1) * m_info.CellSpacing;
}

float Terrain::Depth() const
{
	return (m_info.NumRows - 1) * m_info.CellSpacing;
}

void Terrain::LoadHeightmap()
{
	// A height for each vertex
	std::vector<unsigned char> in(m_info.NumRows * m_info.NumCols);

	// Open the file.
	std::ifstream inFile;
	inFile.open(m_info.HeightmapFilename.c_str(), std::ios_base::binary);

	if (inFile)
	{
		// Read the RAW bytes.
		inFile.read((char*)&in[0], (std::streamsize)in.size());

		// Done with file.
		inFile.close();
	}

	// Copy the array data into a float array, and scale and offset the heights.
	m_vHeightmap.resize(m_info.NumRows * m_info.NumCols, 0);
	for (UINT i = 0; i < m_info.NumRows * m_info.NumCols; ++i)
	{
		m_vHeightmap[i] = (float)in[i] * m_info.HeightScale + m_info.HeightOffset;
	}
}

void Terrain::Smooth()
{
	std::vector<float> dest(m_vHeightmap.size());

	for (UINT i = 0; i < m_info.NumRows; ++i)
	{
		for (UINT j = 0; j < m_info.NumCols; ++j)
		{
			dest[i * m_info.NumCols + j] = Average(i, j);
		}
	}

	// Replace the old heightmap with the filtered one.
	m_vHeightmap = dest;
}

bool Terrain::InBounds(UINT i, UINT j)
{
	// True if ij are valid indices; false otherwise.
	return
	i >= 0 && i < m_info.NumRows &&
	j >= 0 && j < m_info.NumCols;
}

float Terrain::Average(UINT i, UINT j)
{
	// Function computes the average height of the ij element.
	// It averages itself with its eight neighbor pixels.  Note
	// that if a pixel is missing neighbor, we just don't include it
	// in the average--that is, edge pixels don't have a neighbor pixel.
	//
	// ----------
	// | 1| 2| 3|
	// ----------
	// |4 |ij| 6|
	// ----------
	// | 7| 8| 9|
	// ----------

	float avg = 0.0f;
	float num = 0.0f;

	for (UINT m = i - 1; m <= i + 1; ++m)
	{
		for (UINT n = j - 1; n <= j + 1; ++n)
		{
			if (InBounds(m, n))
			{
				avg += m_vHeightmap[m*m_info.NumCols + n];
				num += 1.0f;
			}
		}
	}

	return avg / num;
}

void Terrain::BuildVB()
{
	//std::vector<TerrainVertex> vertices(m_uiNumVertices);

	float halfWidth = (m_info.NumCols - 1)*m_info.CellSpacing*0.5f;
	float halfDepth = (m_info.NumRows - 1)*m_info.CellSpacing*0.5f;

	float du = 1.0f / (m_info.NumCols - 1);
	float dv = 1.0f / (m_info.NumRows - 1);
	for (UINT i = 0; i < m_info.NumRows; ++i)
	{
		float z = halfDepth - i*m_info.CellSpacing;
		for (UINT j = 0; j < m_info.NumCols; ++j)
		{
			float x = -halfWidth + j*m_info.CellSpacing;

			float y = m_vHeightmap[i*m_info.NumCols + j];
			vertices[i*m_info.NumCols + j].pos = glm::vec3(x, y, z);
			vertices[i*m_info.NumCols + j].normal = glm::vec3(0.0f, 1.0f, 0.0f);

			// Stretch texture over grid.
			vertices[i*m_info.NumCols + j].texC.x = j*du;
			vertices[i*m_info.NumCols + j].texC.y = i*dv;
		}
	}

	// Estimate normals for interior nodes using central difference.
	float invTwoDX = 1.0f / (2.0f*m_info.CellSpacing);
	float invTwoDZ = 1.0f / (2.0f*m_info.CellSpacing);
	for (UINT i = 2; i < m_info.NumRows - 1; ++i)
	{
		for (UINT j = 2; j < m_info.NumCols - 1; ++j)
		{
			float t = m_vHeightmap[(i - 1)*m_info.NumCols + j];
			float b = m_vHeightmap[(i + 1)*m_info.NumCols + j];
			float l = m_vHeightmap[i*m_info.NumCols + j - 1];
			float r = m_vHeightmap[i*m_info.NumCols + j + 1];

			glm::vec3 tanZ(0.0f, (t - b)*invTwoDZ, 1.0f);
			glm::vec3 tanX(1.0f, (r - l)*invTwoDX, 0.0f);

			glm::vec3 N = glm::cross(tanZ, tanX);
			N = glm::normalize(N);

			vertices[i*m_info.NumCols + j].normal = N;
		}
	}

}

void Terrain::BuildIB()
{
	//std::vector<DWORD> indices(m_uiNumFaces * 3); // 3 indices per face

											   // Iterate over each quad and compute indices.
	int k = 0;
	for (UINT i = 0; i < m_info.NumRows - 1; ++i)
	{
		for (UINT j = 0; j < m_info.NumCols - 1; ++j)
		{
			indices[k] = i*m_info.NumCols + j;
			indices[k + 1] = i*m_info.NumCols + j + 1;
			indices[k + 2] = (i + 1)*m_info.NumCols + j;

			indices[k + 3] = (i + 1)*m_info.NumCols + j;
			indices[k + 4] = i*m_info.NumCols + j + 1;
			indices[k + 5] = (i + 1)*m_info.NumCols + j + 1;

			k += 6; // next quad
		}
	}

	

}


Terrain::~Terrain()
{
}
