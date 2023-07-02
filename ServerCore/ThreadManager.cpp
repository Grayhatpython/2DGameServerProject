#include "pch.h"
#include "ThreadManager.h"
#include "Thread.h"

ThreadManager::ThreadManager()
{

}

ThreadManager::~ThreadManager()
{
	Clear();
}

void ThreadManager::Join()
{
	for (auto& thread : _threads)
		thread.second->Join();
}

void ThreadManager::Stop()
{
	for (auto& thread : _threads)
		thread.second->Stop();
}

void ThreadManager::Launch(const std::wstring& name, std::function<void()> callback)
{
	std::lock_guard gurad(_lock);

	Thread* thread = new Thread(name, callback);
	std::this_thread::sleep_for(20ms);
	_threads.insert(std::make_pair(thread->GetId(), thread));
}

void ThreadManager::Remove(uint32 threadId)
{
	std::lock_guard gurad(_lock);
	auto iter = _threads.find(threadId);
	if (iter != _threads.end())
		_threads.erase(iter);
}

void ThreadManager::Clear()
{
	Stop();

	for (auto& thread : _threads)
		delete thread.second;

	_threads.clear();
}

Thread* ThreadManager::FindThreadFromId(uint32 threadId)
{
	std::lock_guard gurad(_lock);
	auto iter = _threads.find(threadId);
	if (iter != _threads.end())
		return iter->second;

	return nullptr;
}

Thread* ThreadManager::FindThreadFromName(const std::wstring& name)
{
	std::lock_guard gurad(_lock);
	for (auto thread : _threads)
	{
		if (thread.second->GetName().compare(name) == 0)
			return thread.second;
	}

	return nullptr;
}
