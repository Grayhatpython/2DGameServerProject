#include "pch.h"
#include "MiniDump.h"
#include <DbgHelp.h>

typedef BOOL(WINAPI* MINIDUMPWRITEDUMP)( // Callback 함수의 원형
	HANDLE hProcess,
	DWORD dwPid,
	HANDLE hFile,
	MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

LPTOP_LEVEL_EXCEPTION_FILTER PreviousExceptionFilter = NULL;

LONG WINAPI UnHandleExceptionFilter(struct _EXCEPTION_POINTERS* exceptionInfo)
{
	HMODULE hModule = nullptr;
	hModule = LoadLibrary(L"DBGHELP.DLL");

	if (hModule)
	{
		MINIDUMPWRITEDUMP dump = (MINIDUMPWRITEDUMP)::GetProcAddress(hModule, "MiniDumpWriteDump");
		if (dump)
		{
			WCHAR dumpPath[MAX_PATH] = { 0, };
			SYSTEMTIME systemTime;
			::GetLocalTime(&systemTime);
			
			_snwprintf_s(dumpPath, MAX_PATH, _TRUNCATE, L"%d-%d-%d %d-%d-%d.dmp",
				systemTime.wYear,
				systemTime.wMonth,
				systemTime.wDay,
				systemTime.wHour,
				systemTime.wMinute,
				systemTime.wSecond);

			HANDLE file = CreateFile(dumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			if (file != INVALID_HANDLE_VALUE)
			{
				_MINIDUMP_EXCEPTION_INFORMATION minidumpExceptionInfo;

				minidumpExceptionInfo.ThreadId = ::GetCurrentThreadId();
				minidumpExceptionInfo.ExceptionPointers = exceptionInfo;
				minidumpExceptionInfo.ClientPointers = NULL;

				auto success = dump(::GetCurrentProcess(), ::GetCurrentProcessId(), file, MiniDumpNormal, &minidumpExceptionInfo, NULL, NULL);
				if (success)
				{
					::CloseHandle(file);
					return EXCEPTION_EXECUTE_HANDLER;
				}
			}

			::CloseHandle(file);
		}
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

void MiniDump::Initialize()
{
	::SetErrorMode(SEM_FAILCRITICALERRORS);
	PreviousExceptionFilter = ::SetUnhandledExceptionFilter(UnHandleExceptionFilter);
}

void MiniDump::Dump()
{
	::SetUnhandledExceptionFilter(PreviousExceptionFilter);
}
