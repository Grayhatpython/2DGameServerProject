#pragma once
#include "Job.h"
#include "ConcurrentQueue.h"
#include "JobTimer.h"

class JobBaseObject : public std::enable_shared_from_this<JobBaseObject>
{
public:
	void PushJobAndRandomCanExecute(FuncType&& func)
	{
		Push(ObjectPool<Job>::MakeShared(std::move(func)));
	}

	template<typename T, typename Ret, typename... Args>
	void PushJobAndRandomCanExecute(Ret(T::* memberFunc)(Args...), Args... args)
	{
		static_assert(std::is_base_of<JobBaseObject, T>::value, "Not JobBaseObject");
		std::shared_ptr<T> owner = std::static_pointer_cast<T>(shared_from_this());
		Push(ObjectPool<Job>::MakeShared(owner, memberFunc, std::forward<Args>(args)...));
	}

	void OnlyPushJob(FuncType&& func)
	{
		Push(ObjectPool<Job>::MakeShared(std::move(func)), true);
	}

	template<typename T, typename Ret, typename... Args>
	void OnlyPushJob(Ret(T::* memberFunc)(Args...), Args... args)
	{
		static_assert(std::is_base_of<JobBaseObject, T>::value, "Not JobBaseObject");
		std::shared_ptr<T> owner = std::static_pointer_cast<T>(shared_from_this());
		Push(ObjectPool<Job>::MakeShared(owner, memberFunc, std::forward<Args>(args)...), true);
	}

	void OnlyPushJobAndNotDistribute(FuncType&& func)
	{
		Push(ObjectPool<Job>::MakeShared(std::move(func)), true, false);
	}

	template<typename T, typename Ret, typename... Args>
	void OnlyPushJobAndNotDistribute(Ret(T::* memberFunc)(Args...), Args... args)
	{
		static_assert(std::is_base_of<JobBaseObject, T>::value, "Not JobBaseObject");
		std::shared_ptr<T> owner = std::static_pointer_cast<T>(shared_from_this());
		Push(ObjectPool<Job>::MakeShared(owner, memberFunc, std::forward<Args>(args)...), true, false);
	}

	std::shared_ptr<Job> PushJobTimer(uint64 tick, FuncType&& func)
	{
		auto job = ObjectPool<Job>::MakeShared(std::move(func));
		return GJobTimer->Reserve(tick, shared_from_this(), job);
	}

	template<typename T, typename Ret, typename... Args>
	std::shared_ptr<Job> PushJobTimer(uint64 tick, Ret(T::* memberFunc)(Args...), Args... args)
	{
		static_assert(std::is_base_of<JobBaseObject, T>::value, "Not JobBaseObject");
		std::shared_ptr<T> owner = std::static_pointer_cast<T>(shared_from_this());
		auto job = ObjectPool<Job>::MakeShared(owner, memberFunc, std::forward<Args>(args)...);
		return GJobTimer->Reserve(tick, shared_from_this(), job);
	}

public:
	void	ClearJob() { _jobQueue.Clear(); }

public:
	void	Execute();
	void	KeepExecute();
	void	Push(std::shared_ptr<Job> job, bool isOnlyPush = false, bool isDistribute = true);

protected:
	ConcurrentQueue<std::shared_ptr<Job>>	_jobQueue;
	std::atomic<int32>						_jobCount = 0;
};

