// ObjectMacros.h
// Helper mactors that define common functionality
// needed by components, actors, and assets
// Edit this file at your own peril!

#pragma once

// Use this as DECL_COMPONENT(SelfClass, SuperClass)
#define DECL_COMPONENT(d,s) typedef s Super; \
	std::shared_ptr<d> ThisPtr() \
	{ \
		return std::static_pointer_cast<d>(shared_from_this()); \
	} \
	static const TypeInfo sType; \
	public: \
	static const TypeInfo* StaticType() { return &sType; } \
	const TypeInfo* GetType() const override { return &sType; } \
	static std::shared_ptr<d> Create(class Actor& actor, \
		UpdateType update = PostTick); \
	private: \

// Use this as IMPL_COMPONENT(SelfClass, SuperClass)
#define IMPL_COMPONENT(d,s) \
	const TypeInfo d::sType(s::StaticType()); \
	std::shared_ptr<d> d::Create(Actor& actor, UpdateType update) \
	{\
		std::shared_ptr<d> ptr = std::make_shared<d>(actor); \
		ptr->mOwner.AddComponent(ptr, update); \
		return ptr; \
	}\

// Use this as DECL_ACTOR(SelfClass, SuperClass)
#define DECL_ACTOR(d,s) typedef s Super; \
	std::shared_ptr<d> ThisPtr() \
	{ \
		return std::static_pointer_cast<d>(shared_from_this()); \
	} \
	static const TypeInfo sType; \
	public: \
	static const TypeInfo* StaticType() { return &sType; } \
	const TypeInfo* GetType() const override { return &sType; } \
	static std::shared_ptr<d> Spawn(class Game& game); \
	static std::shared_ptr<d> SpawnAttached(Actor& parent); \
	private: \

// Use this as IMPL_ACTOR(SelfClass, SuperClass)
#define IMPL_ACTOR(d,s) \
	const TypeInfo d::sType(s::StaticType()); \
	std::shared_ptr<d> d::Spawn(Game& game) \
	{\
		std::shared_ptr<d> ptr = std::make_shared<d>(game); \
		game.GetWorld().AddActor(ptr); \
		ptr->BeginPlay(); \
		return ptr; \
	}\
	std::shared_ptr<d> d::SpawnAttached(Actor& parent) \
	{\
		std::shared_ptr<d> ptr = std::make_shared<d>(parent.GetGame()); \
		parent.AddChild(ptr); \
		ptr->BeginPlay(); \
		return ptr; \
	}\


// Use this as DECL_ASSET(SelfClass, SuperClass)
#define DECL_ASSET(d,s) typedef s Super; \
	std::shared_ptr<d> ThisPtr() \
	{ \
		return std::static_pointer_cast<d>(shared_from_this()); \
	} \
	public: \
	static std::shared_ptr<d> StaticLoad(const char* file, class AssetCache* cache) \
	{ \
		std::shared_ptr<d> ptr = std::make_shared<d>(); \
		if (!ptr->Load(file, cache)) { return nullptr; } \
		return ptr; \
	} \
	private: \

// Use this as DECL_PTR(Class)
#define DECL_PTR(o) typedef std::shared_ptr<o> o##Ptr;
