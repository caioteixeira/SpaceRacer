// VertexArray.h
// This is a helper class to wrap OpenGL's
// Vertex array objects
// You shouldn't have to edit this file!

#pragma once
#include "Math.h"
#include <memory>
#include <GL/glew.h>

// For simplicity, we assume a set vertex format
// This isn't generally a realistic assumption
struct Vertex
{
	// Position of vertex
	Vector3 mPos;
	// Vertex normal
	Vector3 mNormal;
	// Texture coordinate of vertex
	Vector2 mTexCoord;

	Vertex(float x, float y, float z, float u, float v)
		:mPos(x, y, z)
		,mTexCoord(u,v)
	{}

	Vertex(const Vector3& pos, const Vector2& texcoord)
		:mPos(pos)
		,mTexCoord(texcoord)
	{}

	Vertex(const Vector3& pos, const Vector3& normal, const Vector2& texcoord)
		:mPos(pos)
		,mNormal(normal)
		,mTexCoord(texcoord)
	{}
};

typedef std::shared_ptr<class VertexArray> VertexArrayPtr;

class VertexArray
{
public:
	static VertexArrayPtr Create(const Vertex* verts, size_t vertCount, const GLuint* indices, size_t indexCount);
	VertexArray(const Vertex* verts, size_t vertCount, const GLuint* indices, size_t indexCount);
public:
	~VertexArray();
	void SetActive();

	GLuint GetVertexCount() const { return static_cast<GLuint>(mVertexCount); }
	GLuint GetIndexCount() const { return static_cast<GLuint>(mIndexCount); }
	GLuint GetVertexBuffer() const { return mVertexBuffer; }
	GLuint GetIndexBuffer() const { return mIndexBuffer; }
	GLuint GetVertexArray() const { return mVertexArray; }
private:
	size_t mVertexCount;
	size_t mIndexCount;
	GLuint mVertexBuffer;
	GLuint mIndexBuffer;
	GLuint mVertexArray;
};
