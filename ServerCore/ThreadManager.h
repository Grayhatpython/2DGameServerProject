#pragma once

class Thread;
class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

public:
	void	Join();
	void	Stop();
	void	Launch(const std::wstring& name, std::function<void()> callback);
	void	Remove(uint32 threadId);
	void	Clear();
	
public:
	Thread* FindThreadFromId(uint32 threadId);
	Thread* FindThreadFromName(const std::wstring& name);

private:
	std::map<uint32, Thread*>	_threads;
	std::mutex					_lock;
};

