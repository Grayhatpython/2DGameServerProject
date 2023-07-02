#include "pch.h"
#include "Logger.h"
#include <fstream>

void Logger::Log(const char* message, ...)
{
	SYSTEMTIME systemTime;
	char date[MAX_PATH] = { 0, };
	char log[1024] = { 0, };
	char debugLog[1024] = { 0, };

	va_list sp;

	va_start(sp, message);
	vsprintf_s(log, message, sp);
	va_end(sp);

	::GetLocalTime(&systemTime);
	_snprintf_s(date, MAX_PATH, _TRUNCATE, "Thread[%d] %d-%d-%d %d:%d:%d", LThreadId, systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
	_snprintf_s(debugLog, 1024, _TRUNCATE, "[%s] : %s\n", date, log);

	OutputDebugStringA(debugLog);
	printf_s("%s", debugLog);
}

void Logger::Log(const WCHAR* message, ...)
{
	SYSTEMTIME systemTime;
	WCHAR date[MAX_PATH] = { 0, };
	WCHAR log[1024] = { 0, };
	WCHAR debugLog[1024] = { 0, };

	va_list sp;

	va_start(sp, message);
	vswprintf_s(log, message, sp);
	va_end(sp);

	::GetLocalTime(&systemTime);
	_snwprintf_s(date, MAX_PATH, _TRUNCATE, _T("Thread[%d] %d-%d-%d %d:%d:%d"), LThreadId, systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
	_snwprintf_s(debugLog, 1024, _TRUNCATE, _T("[%s] : %s\n"), date, log);

	OutputDebugStringW(debugLog);
	wprintf_s(L"%s", debugLog);
}
