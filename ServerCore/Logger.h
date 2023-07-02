#pragma once

class Logger
{
public:
	void Log(const char* message, ...);
	void Log(const WCHAR* message, ...);
};

