#include "MoveComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"

IMPL_COMPONENT(MoveComponent, Component);

MoveComponent::MoveComponent(Actor& owner)
	:Component(owner)
	,mLinearSpeed(0.0f)
	,mYawSpeed(0.0f)
	,mLinearAxis(0.0f)
	,mYawAxis(0.0f)
	,mPitchAxis(0.0f)
	,mPitchSpeed(0.0f)
{

}

void MoveComponent::Tick(float deltaTime)
{
	//Update the rotation
	if (GetYawSpeed() != 0.0f || GetPitchSpeed() != 0.0f)
	{
		Quaternion currentRotation = mOwner.GetRotation();
		//Yaw
		float angle = (GetYawSpeed() * GetYawAxis() * deltaTime);
		Quaternion incRotation = Quaternion(mOwner.GetWorldTransform().GetZAxis(), angle);
		Quaternion newRotation = Concatenate(currentRotation, incRotation);
		//Pitch
		angle = (GetYawSpeed() * GetPitchAxis() * deltaTime);
		incRotation = Quaternion(mOwner.GetWorldTransform().GetYAxis(), angle);
		newRotation = Concatenate(newRotation, incRotation);

		mOwner.SetRotation(newRotation);
	}

	if (!Math::IsZero(mLinearAxis))
	{
		//Update velocity on current forward
		mVelocity = mOwner.GetForward()*(mLinearSpeed*mLinearAxis);
		//Update position of owner based on velocity
		mOwner.SetPosition(mOwner.GetPosition() + (mVelocity*deltaTime));
		
	}
}

void MoveComponent::AddToLinearAxis(float delta)
{
	mLinearAxis += delta;
	mLinearAxis = Math::Clamp(mLinearAxis, -1.0f, 1.0f);
}

void MoveComponent::AddToYawAxis(float delta)
{
	mYawAxis += delta;
	mYawAxis = Math::Clamp(mYawAxis, -1.0f, 1.0f);
}

void MoveComponent::SetLinearAxis(float axis)
{
	mLinearAxis = Math::Clamp(axis, -1.0f, 1.0f);
}

void MoveComponent::SetYawAxis(float axis)
{
	mYawAxis = Math::Clamp(axis, -1.0f, 1.0f);
}


void MoveComponent::AddToPitchAxis(float delta)
{
	mPitchAxis += delta;
	mPitchAxis = Math::Clamp(mPitchAxis, -1.0f, 1.0f);
}

void MoveComponent::SetPitchAxis(float axis)
{
	mPitchAxis = Math::Clamp(axis, -1.0f, 1.0f);
}