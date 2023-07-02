#pragma once

class Thread
{
public:
	Thread(const std::wstring& name, std::function<void()> callback);
	~Thread();

public:
	void				Join();
	void				Stop() { _stopped = true; }

public:
	uint32				GetId() { return _threadId; }
	std::wstring&		GetName() { return _name; }

private:
	uint32				_threadId = 0;
	std::wstring		_name;
	std::thread			_thread;
	bool				_stopped = false;
};