#pragma once
#include <DbgHelp.h>

class ThreadCallStack
{
public:
	ThreadCallStack();
	~ThreadCallStack();

public:
	void Print();
	bool Clear();

private:
	static const int16_t		S_MaxStackDepth = 16;
	void*						_stack[S_MaxStackDepth] = { 0, };
	uint16_t					_frames = 0;
	SYMBOL_INFO*				_symbol = nullptr;;
	uint16_t					_symbolSize = sizeof(SYMBOL_INFO) + MAX_PATH * sizeof(char);
	HANDLE						_process;
	DWORD						_displacement = 0;
	IMAGEHLP_LINE64				_line;

};