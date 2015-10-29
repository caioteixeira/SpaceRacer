#include "CameraComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"

IMPL_COMPONENT(CameraComponent, Component);

CameraComponent::CameraComponent(Actor & owner) : Component(owner)
{
	dampConstant = 2.0f * sqrt(mSpringConstant);
}

void CameraComponent::Tick(float deltaTime){


	//Calculate new camera matrix
	auto ShipUp = mOwner.GetWorldTransform().GetZAxis();
	auto ShipForward = mOwner.GetWorldTransform().GetXAxis();
	ShipForward.Normalize();
	ShipUp.Normalize();

	/*Spring Camera Stuff*/

	Vector3 idealPosition = IdealCameraPosition();

	auto displacement = mActualCameraPosition - idealPosition;
	auto springAccel = (-mSpringConstant * displacement) - (dampConstant * mActualCameraVelocity);
	mActualCameraVelocity += springAccel * deltaTime;
	mActualCameraPosition += mActualCameraVelocity * deltaTime;


	auto targetPosition = mOwner.GetPosition() + ShipForward * mTargetOffset;
	auto vCameraForward = targetPosition - mActualCameraPosition;
	vCameraForward.Normalize();
	auto vCameraLeft = Cross(ShipUp, vCameraForward);
	vCameraLeft.Normalize();
	auto vCameraUp = Cross(vCameraForward, vCameraLeft);
	vCameraUp.Normalize();

	Matrix4 cameraMatrix = Matrix4::CreateLookAt(mActualCameraPosition, targetPosition, vCameraUp);

	//Update renderer view proj
	mOwner.GetGame().GetRenderer().UpdateViewMatrix(cameraMatrix);
}

Vector3 CameraComponent::IdealCameraPosition(){

	float mHFollowDistance = Math::Lerp(mHMinFollowDistance, mHMaxFollowDistance, mMoveComponentPtr->GetLinearAxis());
	//Calculate new camera matrix
	auto ShipForward = mOwner.GetWorldTransform().GetXAxis();
	auto ShipUp = mOwner.GetWorldTransform().GetZAxis();
	ShipForward.Normalize();
	ShipUp.Normalize();

	return  mOwner.GetPosition() - (ShipForward * mHFollowDistance) + (ShipUp * mVFollowDistance);
}