// MoveComponent.h
// Component that does basic velocity-based movement
// Generally used as a "pre-tick" component

#pragma once
#include "Component.h"
#include "Math.h"

class MoveComponent : public Component
{
	DECL_COMPONENT(MoveComponent, Component);
public:
	MoveComponent(Actor& owner);

	void Tick(float deltaTime) override;

	// Linear speed (in game units/s)
	void SetLinearSpeed(float speed) { mLinearSpeed = speed; }
	float GetLinearSpeed() const { return mLinearSpeed; }

	// Angular speed (in radians/s)
	void SetYawSpeed(float speed) { mYawSpeed = speed; }
	float GetYawSpeed() const { return mYawSpeed; }
	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
	float GetPitchSpeed() const { return mPitchSpeed; }

	// AddTo calls should always be +1.0f or -1.0f
	void AddToLinearAxis(float delta);
	void AddToYawAxis(float delta);
	void AddToPitchAxis(float delta);

	void SetLinearAxis(float axis);
	float GetLinearAxis() const { return mLinearAxis; }

	void SetYawAxis(float axis);
	float GetYawAxis() const { return mYawAxis; }
	void SetPitchAxis(float axis);
	float GetPitchAxis() const { return mPitchAxis; }
private:
	Vector3 mVelocity;

	float mLinearSpeed;
	float mYawSpeed;
	float mPitchSpeed;

	float mLinearAxis;
	float mYawAxis;
	float mPitchAxis;
};

DECL_PTR(MoveComponent);
