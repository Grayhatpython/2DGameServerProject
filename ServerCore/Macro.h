#pragma once

#include <Windows.h>

#define _STRING2(x) #x
#define _STRING(x)  _STRING2(x)
#define JUMP_MSG(desc) message(__FILE__ "(" _STRING(__LINE__) "):" #desc)

#define SIZE(str)   ((lstrlen(str) + 1) * sizeof(TCHAR))

#ifdef _X86_
#define DebugBreak()    _asm { int 3 }
#endif

inline void MsgBox(PCSTR msg) {
	char str[MAX_PATH];
	::GetModuleFileNameA(NULL, str, _countof(str));
	::MessageBoxA(::GetActiveWindow(), msg, str, MB_OK);
}

inline void Fail(PSTR message) {
	MsgBox(message);
	::DebugBreak();
}

inline void AssertFail(LPCSTR file, int line, PCSTR cause) {
	char str[2 * MAX_PATH];
	::wsprintfA(str, "File %s, line %d : %s\n", file, line, cause);
	::OutputDebugStringA(str);
	Fail(str);
}

#ifdef _DEBUG
#define ASSERT(cause) if (!(cause)) AssertFail(__FILE__, __LINE__, #cause)
#else
#define ASSERT(cause)				\
{									\
	if(!(cause))					\
	{								\
		int* crashVal = nullptr;	\
		*crashVal = 0xDEADBEEF;		\
	}								\
}

#endif

#define USE_MANY_LOCKS(count)	Lock _locks[count];
#define USE_LOCK				USE_MANY_LOCKS(1)
#define	READ_LOCK_IDX(idx)		ReadLockGuard readLockGuard_##idx(_locks[idx], typeid(this).name());
#define READ_LOCK				READ_LOCK_IDX(0)
#define	WRITE_LOCK_IDX(idx)		WriteLockGuard writeLockGuard_##idx(_locks[idx], typeid(this).name());
#define WRITE_LOCK				WRITE_LOCK_IDX(0)

//	TEMP
#define LOG(message, ...) GLogger->Log(message, __VA_ARGS__)
