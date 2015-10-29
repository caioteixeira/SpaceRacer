#include "InputManager.h"

void InputManager::HandleKeyPressed(int key)
{
	auto iter = mKeyToActionMap.find(key);
	if (iter != mKeyToActionMap.end())
	{
		ActionDelegatePtr delegatePtr = iter->second->mPressedDelegate;
		if (delegatePtr != nullptr)
		{
			delegatePtr->Execute();
		}
	}

	auto axis = mKeyToAxisMap.find(key);
	if (axis != mKeyToAxisMap.end())
	{
		AxisMappingPtr axisPtr = axis->second;
		AxisDelegatePtr delegatePtr = axisPtr->mDelegate;
		if (axisPtr->mPositiveKey == key)
		{
			axisPtr->mValue += 1.0f;
		}
		else if (axisPtr->mNegativeKey == key)
		{
			axisPtr->mValue -= 1.0f;
		}

		if (delegatePtr != nullptr)
		{
			delegatePtr->Execute(axisPtr->mValue);
		}
	}
}

void InputManager::HandleKeyReleased(int key)
{
	auto iter = mKeyToActionMap.find(key);
	if (iter != mKeyToActionMap.end())
	{
		ActionDelegatePtr delegatePtr = iter->second->mReleasedDelegate;
		if (delegatePtr != nullptr)
		{
			delegatePtr->Execute();
		}
	}

	auto axis = mKeyToAxisMap.find(key);
	if (axis != mKeyToAxisMap.end())
	{
		AxisMappingPtr axisPtr = axis->second;
		AxisDelegatePtr delegatePtr = axisPtr->mDelegate;
		if (axisPtr->mPositiveKey == key)
		{
			axisPtr->mValue -= 1.0f;
		}
		else if (axisPtr->mNegativeKey == key)
		{
			axisPtr->mValue += 1.0f;
		}

		if (delegatePtr != nullptr)
		{
			delegatePtr->Execute(axisPtr->mValue);
		}
	}
}

void InputManager::AddActionMapping(const std::string& name, int key)
{
	ActionMappingPtr ptr = std::make_shared<ActionMapping>();
	ptr->mName = name;
	ptr->mKey = key;

	mNameToActionMap.emplace(name, ptr);
	mKeyToActionMap.emplace(key, ptr);
}

void InputManager::AddAxisMapping(const std::string& name, int positiveKey, int negativeKey)
{
	AxisMappingPtr ptr = std::make_shared<AxisMapping>();
	ptr->mName = name;
	ptr->mNegativeKey = negativeKey;
	ptr->mPositiveKey = positiveKey;
	ptr->mValue = 0.0f;

	mNameToAxisMap.emplace(name, ptr);
	mKeyToAxisMap.emplace(negativeKey, ptr);
	mKeyToAxisMap.emplace(positiveKey, ptr);
}

void InputManager::BindActionInternal(const std::string& name, InputEvent event, ActionDelegatePtr delegate)
{
	auto iter = mNameToActionMap.find(name);
	if (iter != mNameToActionMap.end())
	{
		ActionMappingPtr ptr = iter->second;
		if (event == IE_Pressed)
		{
			ptr->mPressedDelegate = delegate;
		} else if (event == IE_Released)
		{
			ptr->mReleasedDelegate = delegate;
		}
	}
}

void InputManager::BindAxisInternal(const std::string& name, AxisDelegatePtr delegate)
{
	auto iter = mNameToAxisMap.find(name);
	if (iter != mNameToAxisMap.end())
	{
		AxisMappingPtr ptr = iter->second;
		ptr->mDelegate = delegate;
	}
}
