#include "GameTimers.h"

GameTimerManager::GameTimerManager()
	:mNextTimerId(0)
	,mAreTimersTicking(false)
{

}

void GameTimerManager::Tick(float deltaTime)
{
	mAreTimersTicking = true;

	//Loop through Active Timers
	
	for (auto & iter : mActiveTimers){
		//Decrement active timers
		if (iter.second.mStatus == Active)
		{
			iter.second.mRemainingTime -= deltaTime;
		}

		//If the timer is ended
		if (GetRemainingTime(iter.second.mHandle) <= 0.0f)
		{
			//Call the delegate
			TimerDelegatePtr ptr = iter.second.mDelegate;
			ptr->Execute();

			//If is a looping timer
			if (iter.second.mIsLooping)
			{
				iter.second.mRemainingTime = iter.second.mDuration;
			}
			else{
				iter.second.mStatus = Cleared;
				mClearedTimers.push_back(iter.second.mHandle);
			}
		}
	}

	//Loop through cleared Timers
	for (auto& iter : mClearedTimers)
	{
		auto info = mActiveTimers.find(iter);
		if (info != mActiveTimers.end())
		{
			RemoveFromObjMap(info->second.mObj, info->second.mHandle);
			mActiveTimers.erase(iter);
		}
	}

	//Clear Cleared timers
	mClearedTimers.clear();

	//Loop through pending timers
	for (auto& iter : mPendingTimers)
	{
		iter.second.mStatus = Active;
		mActiveTimers.emplace(iter.second.mHandle, iter.second);
	}
	mPendingTimers.clear();

	mAreTimersTicking = false;
}

void GameTimerManager::ClearTimer(const TimerHandle& handle)
{
	// Is this pending?
	auto iter = mPendingTimers.find(handle);
	if (iter != mPendingTimers.end())
	{
		// We can just remove this from pending timers
		RemoveFromObjMap(iter->second.mObj, handle);
		mPendingTimers.erase(iter);
	}
	else
	{
		iter = mActiveTimers.find(handle);
		if (iter != mActiveTimers.end())
		{
			iter->second.mStatus = Cleared;
			mClearedTimers.push_back(handle);
		}
	}
}

void GameTimerManager::PauseTimer(const TimerHandle& handle)
{
	// Is this pending?
	auto iter = mPendingTimers.find(handle);
	if (iter != mPendingTimers.end())
	{
		iter->second.mStatus = Paused;
	}
	else
	{
		iter = mActiveTimers.find(handle);
		if (iter != mActiveTimers.end())
		{
			iter->second.mStatus = Paused;
		}
	}
}

void GameTimerManager::UnpauseTimer(const TimerHandle& handle)
{
	// Is this pending?
	auto iter = mPendingTimers.find(handle);
	if (iter != mPendingTimers.end())
	{
		iter->second.mStatus = Pending;
	}
	else
	{
		iter = mActiveTimers.find(handle);
		if (iter != mActiveTimers.end())
		{
			iter->second.mStatus = Active;
		}
	}
}

float GameTimerManager::GetRemainingTime(const TimerHandle& handle)
{
	// Is this pending?
	auto iter = mPendingTimers.find(handle);
	if (iter != mPendingTimers.end())
	{
		return iter->second.mRemainingTime;
	}
	else
	{
		iter = mActiveTimers.find(handle);
		if (iter != mActiveTimers.end())
		{
			return iter->second.mRemainingTime;
		}
	}

	// Unknown timer
	return -1.0f;
}

void GameTimerManager::AddTime(const TimerHandle& handle, float time)
{
	// Is this pending?
	auto iter = mPendingTimers.find(handle);
	if (iter != mPendingTimers.end())
	{
		iter->second.mRemainingTime += time;
	}
	else
	{
		iter = mActiveTimers.find(handle);
		if (iter != mActiveTimers.end())
		{
			iter->second.mRemainingTime += time;
		}
	}
}

void GameTimerManager::ClearAllTimers(Object* obj)
{
	if (obj == nullptr)
		return;
	auto iter = mObjToHandlesMap.find(obj);
	if (iter != mObjToHandlesMap.end())
	{
		for (auto& t : iter->second)
		{
			ClearTimer(t);
		}

		mObjToHandlesMap.erase(iter);
	}
}

void GameTimerManager::SetTimerInternal(TimerHandle& outHandle, Object* obj, TimerDelegatePtr delegate, float duration, bool looping)
{
	outHandle.mValue = mNextTimerId;
	mNextTimerId++;

	//Set Timer Info
	TimerInfo info;
	info.mDelegate = delegate;
	info.mDuration = duration;
	info.mHandle = outHandle;
	info.mIsLooping = looping;
	info.mObj = obj;
	info.mRemainingTime = duration;
	info.mStatus = mAreTimersTicking ? Pending : Active;
	if (mAreTimersTicking){
		mPendingTimers.emplace(outHandle, info);
	}
	else{
		mActiveTimers.emplace(outHandle, info);
	}
	
	if (info.mStatus == Active){
		AddToObjMap(obj, outHandle);
	}
}

void GameTimerManager::AddToObjMap(Object* obj, const TimerHandle& handle)
{
	auto iter = mObjToHandlesMap.find(obj);
	// Do we already know of this object?
	if (iter != mObjToHandlesMap.end())
	{
		iter->second.emplace(handle);
	}
	else
	{
		std::unordered_set<TimerHandle> temp;
		temp.emplace(handle);
		mObjToHandlesMap.emplace(obj, temp);
	}
}

void GameTimerManager::RemoveFromObjMap(Object* obj, const TimerHandle& handle)
{
	auto iter = mObjToHandlesMap.find(obj);
	if (iter != mObjToHandlesMap.end())
	{
		auto handleIter = iter->second.find(handle);
		if (handleIter != iter->second.end())
		{
			iter->second.erase(handleIter);
		}
	}
}
