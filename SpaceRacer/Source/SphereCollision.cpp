#include "SphereCollision.h"
#include "Actor.h"

IMPL_COMPONENT(SphereCollision, CollisionComponent);

SphereCollision::SphereCollision(Actor& owner)
	:CollisionComponent(owner)
	,mOriginalRadius(1.0f)
	,mActualRadius(1.0f)
	,mScale(1.0f)
{

}

void SphereCollision::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	// Update the radius/center from our owning component
	mCenter = mOwner.GetWorldTransform().GetTranslation();
	

	// Scale the radius by the world transform scale
	// (We can assume the scale is uniform, because
	// actors don't allow non-uniform scaling)
	mActualRadius = mOwner.GetWorldTransform().GetScale().x * mOriginalRadius * mScale;
}

bool SphereCollision::Intersects(CollisionComponentPtr other)
{
	if (IsA<SphereCollision>(other))
	{
		return IntersectsSphere(Cast<SphereCollision>(other));
	}
	return false;
}

void SphereCollision::RadiusFromTexture(TexturePtr texture)
{
	mOriginalRadius = static_cast<float>(Math::Max(texture->GetHeight() / 2, 
		texture->GetHeight() / 2));
}

void SphereCollision::RadiusFromMesh(MeshPtr mesh)
{
	mOriginalRadius = mesh->GetRadius();
}

bool SphereCollision::IntersectsSphere(SphereCollisionPtr other)
{
	auto difVector = mCenter - other->mCenter;
	float sqrDistance = difVector.LengthSq();
	//Make Sum
	float sumOfRad = mActualRadius + other->mActualRadius;
	sumOfRad *= sumOfRad; //Squared 

	if (sqrDistance <= sumOfRad)
	{
		return true;
	}

	return false;
}
