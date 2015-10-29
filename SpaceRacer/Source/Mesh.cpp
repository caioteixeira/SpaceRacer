#include "Mesh.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <SDL/SDL_log.h>
#include "AssetCache.h"

Mesh::Mesh()
	:mRadius(0.0f)
{

}

Mesh::~Mesh()
{

}

bool Mesh::Load(const char* fileName, AssetCache* cache)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("File not found: Mesh %s", fileName);
		return false;
	}
	
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		SDL_Log("Mesh %s is not valid json", fileName);
		return false;
	}

	std::string str = doc["metadata"]["type"].GetString();
	int ver = doc["metadata"]["version"].GetInt();

 	// Check the metadata
 	if (!doc["metadata"].IsObject() ||
		str != "itpmesh" ||
 		ver != 2)
 	{
 		SDL_Log("Mesh %s unknown format", fileName);
 		return false;
 	}

	// Load textures
	const rapidjson::Value& textures = doc["textures"];
	if (!textures.IsArray() || textures.Size() < 1)
	{
		SDL_Log("Mesh %s has no textures, there should be at least one", fileName);
		return false;
	}
	
	for (rapidjson::SizeType i = 0; i < textures.Size(); i++)
	{
		mTextures.emplace_back(cache->Load<Texture>(textures[i].GetString()));
		if (mTextures[i] == nullptr)
		{
			// Failed to load this texture, so use the default
			mTextures[i] = cache->Load<Texture>("Textures/Default.png");
		}
	}

	// Load in the vertices
	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1)
	{
		SDL_Log("Mesh %s has no vertices", fileName);
		return false;
	}

	std::vector<Vertex> vertices;
	vertices.reserve(vertsJson.Size());
	mRadius = 0.0f;
	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++)
	{
		// For now, just assume we have 8 elements
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray() || vert.Size() != 8)
		{
			SDL_Log("Unexpected vertex format for %s", fileName);
			return false;
		}

		Vector3 pos(vert[0].GetDouble(), vert[1].GetDouble(), vert[2].GetDouble());
		mRadius = Math::Max(mRadius, pos.LengthSq());
		vertices.emplace_back(pos,
			Vector3(vert[3].GetDouble(), vert[4].GetDouble(), vert[5].GetDouble()),
			Vector2(vert[6].GetDouble(), vert[7].GetDouble()));
	}

	// We were computing length squared earlier
	mRadius = sqrtf(mRadius);

	// Load in the indices
	const rapidjson::Value& indJson = doc["indices"];
	if (!indJson.IsArray() || indJson.Size() < 1)
	{
		SDL_Log("Mesh %s has no indices", fileName);
		return false;
	}

	std::vector<GLuint> indices;
	indices.reserve(indJson.Size() * 3);
	for (rapidjson::SizeType i = 0; i < indJson.Size(); i++)
	{
		const rapidjson::Value& ind = indJson[i];
		if (!ind.IsArray() || ind.Size() != 3)
		{
			SDL_Log("Invalid indices for %s", fileName);
			return false;
		}
		
		indices.emplace_back(ind[0].GetUint());
		indices.emplace_back(ind[1].GetUint());
		indices.emplace_back(ind[2].GetUint());
	}

	// Now create a vertex array
	mVertexArray = VertexArray::Create(vertices.data(), vertices.size(), 
		indices.data(), indices.size());
	return true;
}

TexturePtr Mesh::GetTexture(size_t index)
{
	if (index < mTextures.size())
	{
		return mTextures[index];
	}
	else
	{
		return nullptr;
	}
}
