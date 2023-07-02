#include "pch.h"
#include "ThreadLocalStorage.h"
#include "ThreadCallStack.h"

thread_local uint32									LThreadId = 0;
thread_local uint64									LEndTickCount = 0;
thread_local std::shared_ptr<class SendBufferChunk> LSendBufferChunk;
thread_local bool									LCanReturnSendBufferChunk = true;
thread_local JobBaseObject*							LCurrentJobBaseObject = nullptr;
//thread_local ThreadCallStack						LThreadCallStack;