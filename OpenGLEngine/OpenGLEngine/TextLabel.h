#pragma once
/****************************************************
* Bachelor of Software Engineering
* Media Design School
* Auckland
* New Zealand
*
* (c) 2005 - 2018 Media Design School
*
* File Name	: "TextLabel.h"
* Description	: TextLabel declaration file
* Author		: Vivian Ngo
* Mail			: vivian.ngo7572@mediadesign.school.nz
******************************************************/

#include "Utility.h"
#include "ShaderLoader.h"

#include <map>
#include <ft2build.h> 
#include FT_FREETYPE_H  

struct Character {
	GLuint TextureID;		// Texture ID 
	glm::ivec2  Size;		// Size of glyph 
	glm::ivec2 Bearing;		// Positon of glyph 
	GLuint Advance;			// How far to move for the next character 
};

class TextLabel
{
public: 
	TextLabel() {};
	TextLabel(std::string text, std::string font, glm::vec2 pos);
	~TextLabel() {};

	void Render(); 
	void SetText(std::string newText) { text = newText; }; 
	void SetColor(glm::vec3 newColor) { color = newColor; }; 
	void SetScale(GLfloat newScale) { scale = newScale; }; 
	void SetPosition(glm::vec2 newPosition) { position = newPosition; };
	glm::vec2 GetPosition();
	float GetTextWidth() { return m_widthText; }
	float GetTextHeight() { return m_heightText; }
	void SetIsHovering(bool hover) { m_isHovered = hover; }
	bool GetIsHovering() { return m_isHovered; }


private: 
	std::string text; 
	GLfloat scale; 
	glm::vec3 color; 
	glm::vec2 position;
	GLuint VAO, VBO, textProgram; 
	std::map<GLchar, Character> Characters; 
	static ShaderLoader shaderLoader;

	float m_widthText;
	float m_heightText;
	bool m_isHovered;
};

