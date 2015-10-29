// SphereCollision.h
// Component that adds sphere collision

#pragma once
#include "CollisionComponent.h"
#include "Texture.h"
#include "Mesh.h"
#include "Math.h"

class SphereCollision : public CollisionComponent
{
	DECL_COMPONENT(SphereCollision, CollisionComponent);
public:
	SphereCollision(Actor& owner);
	
	void Tick(float deltaTime) override;
	bool Intersects(CollisionComponentPtr other) override;

	float GetOriginalRadius() const { return mOriginalRadius; }
	float GetActualRadius() const { return mActualRadius; }
	void RadiusFromTexture(TexturePtr texture);
	void RadiusFromMesh(MeshPtr mesh);

	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
private:
	bool IntersectsSphere(std::shared_ptr<SphereCollision> other);

	Vector3 mCenter;

	float mOriginalRadius;
	float mScale;

	float mActualRadius;
};

DECL_PTR(SphereCollision);
