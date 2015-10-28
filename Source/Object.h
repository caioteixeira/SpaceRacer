// Object.h
// Defines base Object class for all Actors and Components,
// as well as some helpers associated with Object, such
// as custom run-time type information
#pragma once
#include <memory>

// Helper class for RTTI
class TypeInfo
{
public:
	TypeInfo(const TypeInfo* super)
		:mSuper(super)
	{}

	bool IsExactly(const TypeInfo* other) const
	{
		return (this == other);
	}

	const TypeInfo* SuperType() const
	{
		return mSuper;
	}
private:
	const TypeInfo* mSuper;
};

typedef std::shared_ptr<class Object> ObjectPtr;

// Base class for all Actors and Components
class Object : public std::enable_shared_from_this<Object>
{
private:
	static const TypeInfo sType;
protected:
	// Change the access level to the protected
	using std::enable_shared_from_this<Object>::shared_from_this;
public:
	static const TypeInfo* StaticType() { return &sType; }
	virtual const TypeInfo* GetType() const { return &sType; }
};

// Returns true if ptr is-a Type
// Usage: IsA<Type>(ptr)
template <typename Other, typename This>
bool IsA(std::shared_ptr<This> ptr)
{
	for (const TypeInfo* t = ptr->GetType();
		t != nullptr;
		t = t->SuperType())
	{
		if (t->IsExactly(Other::StaticType()))
		{
			return true;
		}
	}

	return false;
}

// Returns true if ref is-a Type
// Usage: IsA<Type>(ref)
template <typename Other, typename This>
bool IsA(const This& ref)
{
	for (const TypeInfo* t = ref.GetType();
		t != nullptr;
		t = t->SuperType())
	{
		if (t->IsExactly(Other::StaticType()))
		{
			return true;
		}
	}

	return false;
}

// Casts ptr to Type if valid
// Usage: Cast<Type>(ptr)
template <typename Other, typename This>
std::shared_ptr<Other> Cast(std::shared_ptr<This> ptr)
{
	if (IsA<Other>(ptr))
	{
		return std::static_pointer_cast<Other>(ptr);
	}
	else
	{
		return nullptr;
	}
}

// Casts ref to Type if valid
// Usage: Cast<Type>(ref)
template <typename Other, typename This>
const Other& Cast(const This& ref)
{
	if (IsA<Other>(ref))
	{
		return static_cast<const Other&>(ref);
	}
	else
	{
		return nullptr;
	}
}

#include "ObjectMacros.h"
