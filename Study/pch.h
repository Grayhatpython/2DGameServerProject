#pragma once

using BYTE = unsigned char;
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

#define NOMINMAX  
#include <Windows.h>

#include <unordered_map>
#include <memory>
#include <map>
#include <assert.h>

#include <string>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>

#include <queue>
#include <memory>
#include <mutex>
#include <exception>
#include <iostream>

#include <thread>


#define USE_MANY_LOCKS(count)	Lock _locks[count];
#define USE_LOCK				USE_MANY_LOCKS(1)
#define	WRITE_LOCK_IDX(idx)		WriteLockGuard writeLockGuard_##idx(_locks[idx]);
#define WRITE_LOCK				WRITE_LOCK_IDX(0)

extern thread_local uint32									LThreadId;

#include "Lock.h"
#include "UtilityHelper.h"