#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"
#include <Windows.h>
#include <cstdlib>
#include <cassert>

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_SimpleVelShader = CompileShaders("./Shaders/SimpleVel.vs", "./Shaders/SimpleVel.fs");
	m_SimpleELtimeShader = CompileShaders("./Shaders/SimpleELtime.vs", "./Shaders/SimpleELtime.fs");
	m_CenterParticleShader = CompileShaders("./Shaders/CenterParticle.vs", "./Shaders/CenterParticle.fs");

	//Create VBOs
	CreateVertexBufferObjects();

	/*GenQuadsVBO(1000);
	CreateProxyGeometry();*/
}

void Renderer::CreateVertexBufferObjects()
{
	float size = 0.02f;
	float rect[]
		=
	{
		-size, -size, 0.f, 0.5, //x,y,z, value
		- size,size, 0.f, 0.5,
		size, size, 0.f, 0.5,//Triangle1

		-size, -size, 0.f,  0.5,
		size, size, 0.f, 0.5,
		size, -size, 0.f, 0.5//Triangle2
	};

	glGenBuffers(1, &m_VBORect); //버텍스 버퍼 오브젝트가 성공을 했으면 1보다 큰 수가 리턴
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	//lecture2
	float tri[]
		=
	{
		-1.0, -0.0, 0.f, 1.0, 0.0, 0.f, 0.0, 0.5, 0.f //Triangle1
	};

	glGenBuffers(1, &m_VBOTri); //버텍스 버퍼 오브젝트가 성공을 했으면 1보다 큰 수가 리턴
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTri);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);


	float color[]
		=
	{
		1,0,0,1, //r,g,b,a
		1,0,0,1,
		1,0,0,1,//Triangle1

		1,0,0,1,
		1,0,0,1,
		1,0,0,1//Triangle2
	};
	glGenBuffers(1, &m_VBORectColor); //버텍스 버퍼 오브젝트가 성공을 했으면 1보다 큰 수가 리턴
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

	GenQuadsVBO(10000 ,false, &m_VBOQuads, &m_VBOQuadsCount);
	CreateProxyGeometry();
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = (GLint)strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}
unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight)
{
	std::cout << "Loading bmp file " << imagepath << " ... " << std::endl;
	outWidth = -1;
	outHeight = -1;
	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = NULL;
	fopen_s(&file, imagepath, "rb");
	if (!file)
	{
		std::cout << "Image could not be opened, " << imagepath << " is missing. " << std::endl;
		return NULL;
	}

	if (fread(header, 1, 54, file) != 54)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (header[0] != 'B' || header[1] != 'M')
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1E]) != 0)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1C]) != 24)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	outWidth = *(int*)&(header[0x12]);
	outHeight = *(int*)&(header[0x16]);

	if (imageSize == 0)
		imageSize = outWidth * outHeight * 3;

	if (dataPos == 0)
		dataPos = 54;

	data = new unsigned char[imageSize];

	fread(data, 1, imageSize, file);

	fclose(file);

	std::cout << imagepath << " is succesfully loaded. " << std::endl;

	return data;
}

GLuint Renderer::CreatePngTexture(char * filePath)
{
	//Load Pngs: Load file and decode image.
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);
	if (error != 0)
	{
		lodepng_error_text(error);
		assert(error == 0);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	return temp;
}

GLuint Renderer::CreateBmpTexture(char * filePath)
{
	//Load Bmp: Load file and decode image.
	unsigned int width, height;
	unsigned char * bmp
		= loadBMPRaw(filePath, width, height);

	if (bmp == NULL)
	{
		std::cout << "Error while loading bmp file : " << filePath << std::endl;
		assert(bmp != NULL);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp);

	return temp;
}

float g_Time = 0.f;

void Renderer::Test()
{
	glUseProgram(m_SolidRectShader);

	//int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");

	g_Time += 0.01f;
	if (g_Time > 1.0f)
		g_Time = 0.f;

	GLuint uTime = glGetUniformLocation(m_SolidRectShader, "u_Time");
	glUniform1f(uTime, g_Time);// 1.f);

	GLuint aPos = glGetAttribLocation(m_SolidRectShader, "a_Position");
	GLuint aCol = glGetAttribLocation(m_SolidRectShader, "a_Color");
		
	glEnableVertexAttribArray(aPos);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect); //총 18개의 float point가 들어가있음.
	glVertexAttribPointer(aPos, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0); //그리고 vbo에 들어가 있는 것을 3개씩 꺼내서 sizeof(float)*3씩 뛰어서 읽어라
	//저 자리에 0으로 넣으면 알아서 3칸씩 띄라고 하는구나! 알고 문제없이 작동함
	//두번째 인자가 4라는건 4개씩 읽고, sizeof(float)*4 칸씩 뛰어라

	glEnableVertexAttribArray(aCol);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectColor); //총 18개의 float point가 들어가있음.
	glVertexAttribPointer(aCol, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	//glDisableVertexAttribArray(attribPosition);
	glDisableVertexAttribArray(0); 
	glDisableVertexAttribArray(1);
}

void Renderer::Lecture2()
{
	//glUseProgram(m_SolidRectShader);
	glUseProgram(m_SimpleVelShader);

	glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, m_VBOTri); //총 18개의 float point가 들어가있음.
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads); //총 18개의 float point가 들어가있음.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0); //그리고 vbo에 들어가 있는 것을 3개씩 꺼내서 sizeof(float)*3씩 뛰어서 읽어라
	//저 자리에 0으로 넣으면 알아서 3칸씩 띄라고 하는구나 알고 문제없이 작동함

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuadsCount);

	glDisableVertexAttribArray(0);
}

void Renderer::Lecture3()
{
	glUseProgram(m_SolidRectShader);

	glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, m_VBOTri); //총 18개의 float point가 들어가있음.
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_GridMesh); //총 18개의 float point가 들어가있음.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0); //그리고 vbo에 들어가 있는 것을 3개씩 꺼내서 sizeof(float)*3씩 뛰어서 읽어라
	//저 자리에 0으로 넣으면 알아서 3칸씩 띄라고 하는구나 알고 문제없이 작동함

	//glDrawArrays(GL_TRIANGLES, 0, m_GridMesh_count);
	glDrawArrays(GL_LINE_STRIP, 0, m_GridMesh_count);

	glDisableVertexAttribArray(0);
}

void Renderer::Lecture4()
{
	glUseProgram(m_SimpleVelShader);

	GLuint uTime = glGetUniformLocation(m_SimpleVelShader, "u_Time");
	glUniform1f(uTime, g_Time);// 1.f); //float point 1개 
	g_Time += 0.0001f;

	GLuint aPos = glGetAttribLocation(m_SimpleVelShader, "a_Position");
	GLuint aVel = glGetAttribLocation(m_SimpleVelShader, "a_Vel");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aVel);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads); //총 18개의 float point가 들어가있음.
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0); //그리고 vbo에 들어가 있는 것을 3개씩 꺼내서 sizeof(float)*3씩 뛰어서 읽어라
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float)*3)); //그리고 vbo에 들어가 있는 것을 3개씩 꺼내서 sizeof(float)*3씩 뛰어서 읽어라

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuadsCount);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
}

void Renderer::Lecture5()
{
	glUseProgram(m_SimpleELtimeShader);

	GLuint uTime = glGetUniformLocation(m_SimpleELtimeShader, "u_Time");
	GLuint uRepeat = glGetUniformLocation(m_SimpleELtimeShader, "u_Repeat");

	glUniform1f(uTime, g_Time);// 1.f); //float point 1개 
	g_Time += 0.0001f;

	GLuint aPos = glGetAttribLocation(m_SimpleELtimeShader, "a_Position");
	GLuint aVel = glGetAttribLocation(m_SimpleELtimeShader, "a_Vel");
	GLuint aStartLife = glGetAttribLocation(m_SimpleELtimeShader, "a_StartLife");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aVel);
	glEnableVertexAttribArray(aStartLife);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads); //총 18개의 float point가 들어가있음.
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0); //그리고 vbo에 들어가 있는 것을 3개씩 꺼내서 sizeof(float)*3씩 뛰어서 읽어라
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 3)); //그리고 vbo에 들어가 있는 것을 3개씩 꺼내서 sizeof(float)*3씩 뛰어서 읽어라
	glVertexAttribPointer(aStartLife, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 6));
	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuadsCount);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aStartLife);
}
// .vs코드들을 실행하면, GenQuadsVBO()에서 cpu에 있던걸 gpu로 올리고, Lecture6()함수 윗부분에서 변수들 세팅해주고, 밑에 glDrawArrays() 함수에서 그려줘라 명령
void Renderer::Lecture6()
{
	GLuint shader = m_CenterParticleShader;
	glUseProgram(shader);

	GLuint uTime = glGetUniformLocation(shader, "u_Time");
	//GLuint uRepeat = glGetUniformLocation(shader, "u_Repeat");

	glUniform1f(uTime, g_Time);// 1.f); //float point 1개 
	g_Time += 0.0001f;

	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aVel = glGetAttribLocation(shader, "a_Vel");
	GLuint aStartLife = glGetAttribLocation(shader, "a_StartLife");
	GLuint aRandValue = glGetAttribLocation(shader, "a_RandValue");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aVel);
	glEnableVertexAttribArray(aStartLife);
	glEnableVertexAttribArray(aRandValue);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads); //총 18개의 float point가 들어가있음.
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 11, 0); //그리고 vbo에 들어가 있는 것을 3개씩 꺼내서 sizeof(float)*3씩 뛰어서 읽어라
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (GLvoid*)(sizeof(float)*3)); //그리고 vbo에 들어가 있는 것을 3개씩 꺼내서 sizeof(float)*3씩 뛰어서 읽어라
	glVertexAttribPointer(aStartLife, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (GLvoid*)(sizeof(float) * 6)); //그리고 vbo에 들어가 있는 것을 3개씩 꺼내서 sizeof(float)*3씩 뛰어서 읽어라
	glVertexAttribPointer(aRandValue, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (GLvoid*)(sizeof(float) * 7)); //그리고 vbo에 들어가 있는 것을 3개씩 꺼내서 sizeof(float)*3씩 뛰어서 읽어라
	
	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuadsCount);// 그려라

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aStartLife);
	glDisableVertexAttribArray(aRandValue);
}

void Renderer::GenQuadsVBO(int count, bool bRandPos, GLuint *id, GLuint *vcount)
{

	int verticesPerQuad = 6; //쿼드 하나 만들라고 버텍스 6개 쓸꺼다
	int floatsPerVertex = 3 + 3 + 2+ 2 + 1; //x,y,z, vx, vy ,vz, s,l, r,a,randVal
	int countQuad = count;

	float size = 0.01f;
	float arraySize = countQuad * verticesPerQuad *floatsPerVertex; //그려야 할 버텍스의 갯수
	float *vertices = new float[arraySize];

	for (int i = 0; i < countQuad; i++)
	{
		float randX, randY, randZ;
		float randVelX, randVelY, randVelZ;
		float startTime, lifeTime;
		float startTimeMax = 6.0f;
		float lifeTimeMax = 3.0f;

		float ratio, amp;
		float ratioMin = 2.f;
		float ratioThres = 4.f;
		float ampMin = -0.1f;
		float ampThres = 0.2f;

		float randValue = 0.f, randThres = 1.f;

		int index = i * verticesPerQuad * floatsPerVertex; //쿼드당 필요한 float 개수씩 건너뜀

		if (bRandPos)
		{
			randX = 2.f*(((float)rand() / (float)RAND_MAX) - 0.5f);
			randY = 2.f*(((float)rand() / (float)RAND_MAX) - 0.5f);
		}
		else
		{
			randX = 0.0f;
			randY = 0.0f;
		}
		startTime = ((float)rand() / (float)RAND_MAX) * startTimeMax;
		lifeTime = ((float)rand() / (float)RAND_MAX) * lifeTimeMax;
		
		randVelX = 2.f*(((float)rand() / (float)RAND_MAX) - 0.5f);
		randVelY = 2.f*(((float)rand() / (float)RAND_MAX) - 0.5f);
		randVelZ = 0; //일단 2D고 이상한데로 갈까봐 0으로

		ratio = ratioMin + ((float)rand() / (float)RAND_MAX) * ratioThres;
		amp = ampMin + ((float)rand() / (float)RAND_MAX) * ampThres;

		randValue = randValue + ((float)rand() / (float)RAND_MAX) * randThres;

		/*
		vertices[index] = randX - size; index++;
		vertices[index] = randY - size; index++;
		vertices[index] = 0.f; index++;
		//vertices[index] = (float)i; index++;

		vertices[index] = randX - size; index++;
		vertices[index] = randY + size; index++;
		vertices[index] = 0.f; index++;
		//vertices[index] = (float)i; index++;

		vertices[index] = randX + size; index++;
		vertices[index] = randY + size; index++;
		vertices[index] = 0.f; index++;
		//vertices[index] = (float)i; index++;

		vertices[index] = randX - size; index++;
		vertices[index] = randY - size; index++;
		vertices[index] = 0.f; index++;
		//vertices[index] = (float)i; index++;

		vertices[index] = randX + size; index++;
		vertices[index] = randY + size; index++;
		vertices[index] = 0.f; index++;
		//vertices[index] = (float)i; index++;

		vertices[index] = randX + size; index++;
		vertices[index] = randY - size; index++;
		vertices[index] = 0.f; index++;
		//vertices[index] = (float)i; index++;
		*/

		vertices[index] = randX - size; index++;
		vertices[index] = randY - size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = randVelX; index++;
		vertices[index] = randVelY; index++;
		vertices[index] = randVelZ; index++;
		vertices[index] = startTime; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = ratio; index++;
		vertices[index] = amp; index++;
		vertices[index] = randValue; index++;

		vertices[index] = randX - size; index++;
		vertices[index] = randY + size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = randVelX; index++;
		vertices[index] = randVelY; index++;
		vertices[index] = randVelZ; index++;
		vertices[index] = startTime; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = ratio; index++;
		vertices[index] = amp; index++;
		vertices[index] = randValue; index++;

		vertices[index] = randX + size; index++;
		vertices[index] = randY + size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = randVelX; index++;
		vertices[index] = randVelY; index++;
		vertices[index] = randVelZ; index++;
		vertices[index] = startTime; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = ratio; index++;
		vertices[index] = amp; index++;
		vertices[index] = randValue; index++;

		vertices[index] = randX - size; index++;
		vertices[index] = randY - size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = randVelX; index++;
		vertices[index] = randVelY; index++;
		vertices[index] = randVelZ; index++;
		vertices[index] = startTime; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = ratio; index++;
		vertices[index] = amp; index++;
		vertices[index] = randValue; index++;

		vertices[index] = randX + size; index++;
		vertices[index] = randY + size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = randVelX; index++;
		vertices[index] = randVelY; index++;
		vertices[index] = randVelZ; index++;
		vertices[index] = startTime; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = ratio; index++;
		vertices[index] = amp; index++;
		vertices[index] = randValue; index++;

		vertices[index] = randX + size; index++;
		vertices[index] = randY - size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = randVelX; index++;
		vertices[index] = randVelY; index++;
		vertices[index] = randVelZ; index++;
		vertices[index] = startTime; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = ratio; index++;
		vertices[index] = amp; index++;
		vertices[index] = randValue; index++;

	}

	GLuint vboID = 0;

	glGenBuffers(1, &vboID); //cpu에서 gpu로 접근하는 id
	glBindBuffer(GL_ARRAY_BUFFER, vboID); //array형태의 자료구조로 사용하겠ㄷ다
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * arraySize, vertices, GL_STATIC_DRAW); //위에서 만든 행렬을 버퍼에 복사
	
	*vcount = countQuad * verticesPerQuad;
	*id = vboID;

	delete(vertices);
}


void Renderer::CreateProxyGeometry()
{
	float basePosX = -0.5f;
	float basePosY = -0.5f;
	float targetPosX = 0.5f;
	float targetPosY = 0.5f;

	int pointCountX = 32;
	int pointCountY = 32;

	float width = targetPosX - basePosX;
	float height = targetPosY - basePosY;

	float* point = new float[pointCountX*pointCountY * 2];
	float* vertices = new float[(pointCountX - 1)*(pointCountY - 1) * 2 * 3 * 3];
	m_GridMesh_count = (pointCountX - 1)*(pointCountY - 1) * 2 * 3;

	//Prepare points
	for (int x = 0; x < pointCountX; x++)
	{
		for (int y = 0; y < pointCountY; y++)
		{
			point[(y*pointCountX + x) * 2 + 0] = basePosX + width * (x / (float)(pointCountX - 1));
			point[(y*pointCountX + x) * 2 + 1] = basePosY + height * (y / (float)(pointCountY - 1));
		}
	}

	//Make triangles
	int vertIndex = 0;
	for (int x = 0; x < pointCountX - 1; x++)
	{
		for (int y = 0; y < pointCountY - 1; y++)
		{
			//Triangle part 1
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

			//Triangle part 2
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
		}
	}

	glGenBuffers(1, &m_VBO_GridMesh);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_GridMesh);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(pointCountX - 1)*(pointCountY - 1) * 2 * 3 * 3, vertices, GL_STATIC_DRAW);
}

