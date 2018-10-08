#pragma once
/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name	: "Terrain.h"
* Description	: Terrain Declaration file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "Utility.h"

namespace
{
	struct TerrainVertex
	{
		glm::vec3 pos;
		glm::vec3 normal;
	};
}

class Terrain
{
public:
	struct InitInfo
	{
		std::string HeightmapFilename;

		float HeightScale;
		float HeightOffset;
		UINT NumRows;
		UINT NumCols;
		float CellSpacing;
	};


	Terrain();
	~Terrain() {};

	void InitialiseTerrain();
	void RenderTerrain();
	float GetHeight(glm::vec3 _position)const;

	float Width()const;
	float Depth()const;

	void LoadHeightmap();
	void Smooth();
	bool InBounds(UINT i, UINT j);
	float Average(UINT i, UINT j);
	void BuildVB();
	void BuildIB();
	int GetFileLength(std::istream& file);

	void SetMeshOn(bool _meshOn) { m_meshOn = _meshOn; }
	bool GetIsMeshOn() { return m_meshOn; }

private:
	InitInfo m_info;

	UINT m_uiNumVertices;
	UINT m_uiNumFaces;
	GLuint m_vbo, m_vao, m_ebo, m_texture, m_program;

	std::vector<float> m_vHeightmap;

	GLsizei indicesSize;
	std::vector<TerrainVertex> vertices;
	std::vector<UINT> indices; // 3 indices per face

	bool m_meshOn;
};

