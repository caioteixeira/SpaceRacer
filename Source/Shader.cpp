#include "Shader.h"
#include <fstream>
#include <sstream>
#include <SDL/SDL.h>

Shader::Shader()
	:mVertexShader(0)
	,mFragmentShader(0)
	,mShaderProgram(0)
{

}

Shader::~Shader()
{
	glDeleteProgram(mShaderProgram);
	glDeleteShader(mVertexShader);
	glDeleteShader(mFragmentShader);
}

void Shader::SetActive()
{
	glUseProgram(mShaderProgram);
}

void Shader::BindViewProjection(const Matrix4& viewProj)
{
	mMatrixBlock.mViewProj = viewProj;
}

void Shader::BindWorldTransform(const Matrix4& worldTransform)
{
	mMatrixBlock.mWorldTransform = worldTransform;
}

void Shader::UploadUniformsToGPU()
{
	// Bind this buffer to index 0
	glBindBuffer(GL_UNIFORM_BUFFER, mUniformBuffer);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, mUniformBuffer);
	
	// Copy uniform buffer data
	GLvoid* p = glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(mMatrixBlock),
					 GL_MAP_INVALIDATE_BUFFER_BIT|GL_MAP_WRITE_BIT);
	memcpy(p, &mMatrixBlock, sizeof(mMatrixBlock));
	glUnmapBuffer(GL_UNIFORM_BUFFER);
}

void Shader::BindTexture(const char* param, TexturePtr texture, int unit)
{
	glActiveTexture(GL_TEXTURE0);
	texture->SetActive();

	GLuint uniform = glGetUniformLocation(mShaderProgram, param);
	glUniform1i(uniform, unit);
}

bool Shader::Load(const char* fileName, class AssetCache* cache)
{
	// Shaders are specified without the extension, we need to add them...
	std::string fileStr(fileName);

	// Vertex shader
	std::ifstream vertexFile(fileStr + ".vert");
	if (vertexFile.is_open())
	{
		std::stringstream sstream;
		sstream << vertexFile.rdbuf();
		mVertexShader = glCreateShader(GL_VERTEX_SHADER);
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();
		glShaderSource(mVertexShader, 1, &(contentsChar), nullptr);
		glCompileShader(mVertexShader);

		if (!IsCompiled(mVertexShader))
		{
			SDL_Log("Failed to compile vertex shader for %s", fileName);
			return false;
		}
	}
	else
	{
		SDL_Log("File not found: Vertex shader for %s", fileName);
		return false;
	}

	// Fragment shader
	std::ifstream fragFile(fileStr + ".frag");
	if (fragFile.is_open())
	{
		std::stringstream sstream;
		sstream << fragFile.rdbuf();
		mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();
		glShaderSource(mFragmentShader, 1, &(contentsChar), nullptr);
		glCompileShader(mFragmentShader);

		if (!IsCompiled(mFragmentShader))
		{
			SDL_Log("Failed to compile fragment shader for %s", fileName);
			return false;
		}
	}
	else
	{
		SDL_Log("File not found: Fragment shader for %s", fileName);
		return false;
	}

	// Now make the combined shader program
	mShaderProgram = glCreateProgram();
	glAttachShader(mShaderProgram, mVertexShader);
	glAttachShader(mShaderProgram, mFragmentShader);
	// assume that the fragment shader always uses "outColor" as the output
	glBindFragDataLocation(mShaderProgram, 0, "outColor");
	glLinkProgram(mShaderProgram);

	if (!IsValidProgram())
	{
		SDL_Log("Shader program %s is invalid.", fileName);
		return false;
	}

	SetActive();

	// Now setup the uniform buffer object
	glGenBuffers(1, &mUniformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, mUniformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mMatrixBlock), &mMatrixBlock, GL_DYNAMIC_DRAW);

	// Bind it to the program
	GLuint block = glGetUniformBlockIndex(mShaderProgram, "MatrixBlock");
	glUniformBlockBinding(mShaderProgram, block, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, mUniformBuffer);

	return true;
}

bool Shader::IsCompiled(GLuint shader)
{
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 512, nullptr, buffer);
		SDL_Log("GLSL Compile Failed:\n%s", buffer);
		return false;
	}

	return true;
}

bool Shader::IsValidProgram()
{
	char buffer[512];
	memset(buffer, 0, 512);

	GLint status;
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		glGetProgramInfoLog(mShaderProgram, 512, nullptr, buffer);
		SDL_Log("GLSL Link Status:\n%s", buffer);
		return false;
	}

//	glValidateProgram(mShaderProgram);
//	glGetProgramiv(mShaderProgram, GL_VALIDATE_STATUS, &status);
//	if (status != GL_TRUE)
//	{
//		glGetProgramInfoLog(mShaderProgram, 512, nullptr, buffer);
//		SDL_Log("GLSL Validate Status:\n%s", buffer);
//		// If this is just a warning, don't die
//		if (buffer[0] != 'W')
//		{
//			return false;
//		}
//	}
	return true;
}
