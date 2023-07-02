#include "pch.h"
#include "Thread.h"
#include "ThreadManager.h"

Thread::Thread(const std::wstring& name, std::function<void()> callback)
	: _name(name)
{
	_thread = std::thread([=]()
		{
			LThreadId = ::GetCurrentThreadId();
			_threadId = LThreadId;
			::SetThreadDescription(::GetCurrentThread(), name.c_str());

			while(_stopped == false)
				callback();

			std::wcout << LThreadId << " " << _name << std::endl;
			LCanReturnSendBufferChunk = false;
			LCurrentJobBaseObject = nullptr;
			LSendBufferChunk = nullptr;
		});
}

Thread::~Thread()
{
	Join();
}

void Thread::Join()
{
	if (_thread.joinable())
		_thread.join();
}
