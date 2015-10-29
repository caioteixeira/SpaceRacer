// Actor.h
// Base class for all "actors" in the world --
// equivalent of "GameObject" in other engines
// Actors can have any number of components,
// as well as child actors.

#pragma once
#include <memory>
#include <unordered_set>
#include "Object.h"
#include "Component.h"
#include "Math.h"

class Game;

typedef std::shared_ptr<class Actor> ActorPtr;

class Actor : public Object
{
	DECL_ACTOR(Actor, Object);
public:
	friend class World;
	
	Actor(Game& game);
	virtual ~Actor();

	// BeginPlay is triggered when an actor spawns
	virtual void BeginPlay();
	// Tick the actor every frame
	virtual void Tick(float deltaTime);
	// EndPlay is triggered when an actor is about to be destroyed
	virtual void EndPlay();
	// BeginTouch is triggered when an actor w/ a collision component
	// collides with another actor
	virtual void BeginTouch(Actor& other);

	Game& GetGame() { return mGame; }

	// Add the requested component with the specified update type
	// Component::PreTick will update prior to Actor::Tick
	// Component::PostTick (default) will update after Actor::Tick
	void AddComponent(ComponentPtr component, Component::UpdateType update = Component::PostTick);
	void RemoveComponent(ComponentPtr component);

	void AddChild(ActorPtr child);
	void RemoveChild(ActorPtr child);

	void SetPosition(const Vector3& pos) { mPosition = pos; ComputeWorldTransform(); }
	const Vector3& GetPosition() const { return mPosition; }

	void SetScale(float scale) { mScale = scale; ComputeWorldTransform(); }
	float GetScale() const { return mScale; }

	void SetRotation(const Quaternion& rotation) { mRotation = rotation; ComputeWorldTransform(); }
	const Quaternion& GetRotation() const { return mRotation; }

	const Matrix4& GetWorldTransform() { return mWorldTransform; }

	Vector3 GetForward() const;

	void SetIsPaused(bool isPaused){ mIsPaused = isPaused; };
	bool GetIsPaused(){ return mIsPaused; };

	void SetIsAlive(bool isAlive) { mIsAlive = isAlive; }
	bool IsAlive() const { return mIsAlive; }
protected:
	// Recomputes the world transform based on this Actor's position/scale/rotation,
	// as well as any parent Actors
	void ComputeWorldTransform();
	Game& mGame;

	std::unordered_set<ComponentPtr> mPreTickComponents;
	std::unordered_set<ComponentPtr> mPostTickComponents;
	std::unordered_set<ActorPtr> mChildren;

	// We don't use a shared pointer here to avoid
	// a circular reference (would be more correct to use a weak_ptr)
	Actor* mParent;
private:
	Matrix4 mWorldTransform;
	Vector3 mPosition;
	float mScale;
	Quaternion mRotation;
	bool mIsPaused;
	bool mIsAlive;
private:
	void TickInternal(float deltaTime);
	void RemoveAllComponents();
	void RemoveAllChildren();
};

// For subclasses, we will normally use DECL_PTR like this
//DECL_PTR(Actor);
