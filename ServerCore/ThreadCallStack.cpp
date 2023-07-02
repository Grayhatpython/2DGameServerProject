#include "pch.h"
#include "ThreadCallStack.h"
#include "Thread.h"
#include "ThreadManager.h"
ThreadCallStack::ThreadCallStack()
{
	_symbol = reinterpret_cast<SYMBOL_INFO*>(::calloc(_symbolSize, 1));
}

ThreadCallStack::~ThreadCallStack()
{
	if (_symbol)
	{
		::free(_symbol);
		_symbol = nullptr;
	}
}

void ThreadCallStack::Print()
{
	if (Clear() == false)
		return;

	_process = ::GetCurrentProcess();
	::SymInitialize(_process, NULL, TRUE);

	_frames = CaptureStackBackTrace(0, S_MaxStackDepth, _stack, NULL);

	if (_symbol == nullptr)
		return;

	_symbol->MaxNameLen = MAX_PATH;
	_symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

	std::cout << "==========================================================================================" << std::endl;
	auto& name = GThreadManager->FindThreadFromId(LThreadId)->GetName();
	std::wcout << name << L" ThreadId : " << LThreadId << L" | Call Stack" << std::endl;
	for (uint16_t i = 0; i < _frames; i++)
	{
		::SymFromAddr(_process, reinterpret_cast<DWORD64>(_stack[i]), 0, _symbol);

		BOOL ret = ::SymGetLineFromAddr64(_process, reinterpret_cast<DWORD64>(_stack[i]), &_displacement, &_line);

		if (ret)
		{
			std::cout << _line.FileName << _line.LineNumber << " - ";
			std::cout << _symbol->Name << std::endl;
			//std::cout << std::hex << symbol->Address << std::endl;
		}
		else
		{
			std::cout << _symbol->Name << std::endl;
			//std::cout << std::hex << symbol->Address << std::endl;
		}
	}
	std::cout << "==========================================================================================" << std::endl;
}

bool ThreadCallStack::Clear()
{
	if (_symbol == nullptr)
		return false;

	::memset(_symbol, 1, _symbolSize);
	::memset(_stack, 0, S_MaxStackDepth * sizeof(void*));
	_frames = 0;
	_displacement = 0;
	_line = {};

	return true;
}
