#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#define NOMINMAX

#include "Types.h"
#include "ThreadLocalStorage.h"
#include "Global.h"
#include "STLContainer.h"
#include "Macro.h"
#include "ThreadCallStack.h"

#include <Windows.h>
#include <iostream>

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "dbghelp.lib")

#include <tchar.h>

#include <cassert>
#include <type_traits>

#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>
#include <functional>

#include "Lock.h"
#include "ObjectPool.h"

#include "SendBuffer.h"
#include "Session.h"
#include "ConcurrentQueue.h"
#include "JobBaseObject.h"

#include "MiniDump.h"
#include "Logger.h"
#include "UtilityHelper.h"
