#pragma once
#include "Asset.h"
#include "VertexArray.h"
#include "Texture.h"
#include <vector>

class Mesh : public Asset
{
	DECL_ASSET(Mesh, Asset);
public:
	Mesh();
	virtual ~Mesh();

	bool Load(const char* fileName, class AssetCache* cache) override;

	VertexArrayPtr GetVertexArray() { return mVertexArray; }
	TexturePtr GetTexture(size_t index);

	float GetRadius() const { return mRadius; }
private:
	VertexArrayPtr mVertexArray;
	std::vector<TexturePtr> mTextures;
	float mRadius;
};

DECL_PTR(Mesh);
