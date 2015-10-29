#pragma once
#include "Component.h"
#include "MoveComponent.h"

class CameraComponent : public Component{
	DECL_COMPONENT(CameraComponent, Component);

public:
	CameraComponent(Actor & owner);
	void Tick(float deltaTime) override;
	void SetMoveComponent(MoveComponentPtr move) { mMoveComponentPtr = move; };
	void SnapToIdeal() { mActualCameraPosition = IdealCameraPosition(); };
	void Recenter();
private:
	float mHMinFollowDistance = 160.0f;
	float mHMaxFollowDistance = 210.0f;
	float mVFollowDistance = 75.0f;
	float mTargetOffset = 150.0f;
	MoveComponentPtr mMoveComponentPtr;

	float mSpringConstant = 150.0f;
	float dampConstant;
	Vector3 IdealCameraPosition();
	Vector3 mActualCameraPosition;
	Vector3 mActualCameraVelocity = Vector3(0.0f,0.0f,0.0f);
};

DECL_PTR(CameraComponent);