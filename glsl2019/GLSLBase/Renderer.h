#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies\glew.h"
#include "Dependencies\wglew.h"
#include "Dependencies\glm/glm.hpp"
#include "Dependencies\glm/gtc/matrix_transform.hpp"
#include "Dependencies\glm/gtx/euler_angles.hpp"

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	GLuint CreatePngTexture(char * filePath);
	GLuint CreateBmpTexture(char * filePath);
	   
	void Test();

	void Lecture2(); //»ï°¢Çü
	void Lecture3();
	void Lecture4();

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects(); 
	unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight);

	void GenQuadsVBO(int count);//·£´ýÇÑ À§Ä¡¿¡ »ï°¢Çü 1000°³
	void CreateProxyGeometry();

	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_VBORect = 0;
	GLuint m_SolidRectShader = 0;
	GLuint m_SimpleVelShader = 0;

	GLuint m_VBOTri = 0;
	GLuint m_VBOQuads = 0;
	int m_VBOQuadsCount = 0;
	
	
	GLuint m_VBO_GridMesh = 0; 
	int m_GridMesh_count = 0;

	GLuint m_VBORectColor = 0;
};

