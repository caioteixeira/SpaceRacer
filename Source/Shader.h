// Shader.h
// Shader asset that encapsulates both the vertex
// and fragment shaders

#pragma once
#include "Asset.h"
#include <GL/glew.h>
#include "Math.h"
#include "Texture.h"

class Shader : public Asset
{
	DECL_ASSET(Shader, Asset);
public:
	Shader();
	virtual ~Shader();

	void SetActive();

	// NOTE: Bind functions assume that this shader is active
	void BindViewProjection(const Matrix4& viewProj);
	void BindWorldTransform(const Matrix4& worldTransform);

	void UploadUniformsToGPU();

	// Bind the specified texture to the specified unit
	void BindTexture(const char* param, TexturePtr texture, int unit);
protected:
	bool Load(const char* fileName, class AssetCache* cache) override;
	bool IsCompiled(GLuint shader);
	bool IsValidProgram();
private:
	// Used for uniform buffer object format (hard-coded for now)
	struct MatrixBlock
	{
		Matrix4 mViewProj;
		Matrix4 mWorldTransform;
	};

	MatrixBlock mMatrixBlock;

	GLuint mVertexShader;
	GLuint mFragmentShader;
	GLuint mShaderProgram;
	GLuint mUniformBuffer;
};

DECL_PTR(Shader);
