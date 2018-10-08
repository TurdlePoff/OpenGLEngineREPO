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
		glm::vec2 texC;
	};
}

class Terrain
{
public:
	struct InitInfo
	{
		std::string HeightmapFilename;
		std::string LayerMapFilename0;
		std::string LayerMapFilename1;
		std::string LayerMapFilename2;
		std::string LayerMapFilename3;
		std::string LayerMapFilename4;
		std::string BlendMapFilename;

		float HeightScale;
		float HeightOffset;
		UINT NumRows;
		UINT NumCols;
		float CellSpacing;
	};


	Terrain();
	~Terrain();

	void InitialiseTerrain();
	void RenderTerrain();
	float GetHeight(float x, float z)const;

	float Width()const;
	float Depth()const;

	void LoadHeightmap();
	void Smooth();
	bool InBounds(UINT i, UINT j);
	float Average(UINT i, UINT j);
	void BuildVB();
	void BuildIB();

private:
	InitInfo m_info;

	UINT m_uiNumVertices;
	UINT m_uiNumFaces;
	GLuint m_vbo, m_vao, m_ebo, m_texture, m_program;

	std::vector<float> m_vHeightmap;

	std::vector<TerrainVertex> vertices;
	std::vector<UINT> indices; // 3 indices per face

};
