#include "Actor.h"
#include "Game.h"

IMPL_ACTOR(Actor, Object);

Actor::Actor(Game& game)
	:mGame(game)
	,mParent(nullptr)
	,mScale(1.0f)
	,mRotation()
	,mIsAlive(true)
	,mIsPaused(false)
{

}

Actor::~Actor()
{
	mGame.GetTime().ClearAllTimers(this);
	RemoveAllChildren();
	RemoveAllComponents();
}

void Actor::BeginPlay()
{

}

void Actor::Tick(float deltaTime)
{

}

void Actor::EndPlay()
{

}

void Actor::BeginTouch(Actor& other)
{

}

void Actor::AddComponent(ComponentPtr component, Component::UpdateType update)
{
	component->Register();
	if (update == Component::PostTick)
	{
		mPostTickComponents.emplace(component);
	}
	else
	{
		mPreTickComponents.emplace(component);
	}
}

void Actor::RemoveComponent(ComponentPtr component)
{
	component->Unregister();

	// This may be pre-tick or post-tick
	auto iter = mPreTickComponents.find(component);
	if (iter != mPreTickComponents.end())
	{
		mPreTickComponents.erase(component);
	}

	iter = mPostTickComponents.find(component);
	if (iter != mPreTickComponents.end())
	{
		mPostTickComponents.erase(component);
	}
}

void Actor::AddChild(ActorPtr child)
{
	mChildren.emplace(child);
	child->mParent = this;
	// Force the child to compute their transform matrix
	child->ComputeWorldTransform();
}

void Actor::RemoveChild(ActorPtr child)
{
	auto iter = mChildren.find(child);
	if (iter != mChildren.end())
	{
		(*iter)->EndPlay();
		mChildren.erase(iter);
	}
	child->mParent = nullptr;
}

Vector3 Actor::GetForward() const
{
	// Following Unreal coordinate system so X is forward
	return mWorldTransform.GetXAxis();
}

void Actor::ComputeWorldTransform()
{
	Matrix4 scaleMatrix = Matrix4::CreateScale(GetScale());
	Matrix4 rotationMatrix = Matrix4::CreateFromQuaternion(GetRotation());
	Matrix4 translationMatrix = Matrix4::CreateTranslation(GetPosition());
	mWorldTransform = scaleMatrix * rotationMatrix * translationMatrix;

	//If the Actor has a parent
	if (mParent)
	{
		mWorldTransform *= mParent->GetWorldTransform();
	}

	for (auto & child : mChildren)
	{
		child->ComputeWorldTransform();
	}
}

void Actor::TickInternal(float deltaTime)
{
	//if paused, don't update anything
	if (mIsPaused)
		return;

	// Tick each component in pre-tick set
	for (ComponentPtr component : mPreTickComponents)
	{
		//Tick
		component->Tick(deltaTime);
	}

	// Tick the Actor
	Tick(deltaTime);
	
	// Tick each component in post-tick set
	for (ComponentPtr component : mPostTickComponents)
	{
		//Tick
		component->Tick(deltaTime);
	}

	// Tick Internal each child actor
	for (ActorPtr child : mChildren)
	{
		child->TickInternal(deltaTime);
	}
}

void Actor::RemoveAllComponents()
{
	// Unregister everything first
	for (auto& comp : mPreTickComponents)
	{
		comp->Unregister();
	}

	for (auto& comp : mPostTickComponents)
	{
		comp->Unregister();
	}

	mPreTickComponents.clear();
	mPostTickComponents.clear();
}

void Actor::RemoveAllChildren()
{
	for (auto& child : mChildren)
	{
		child->EndPlay();
		child->mParent = nullptr;
	}

	mChildren.clear();
}
