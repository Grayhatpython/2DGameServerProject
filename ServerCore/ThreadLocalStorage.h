#pragma once
#include <memory>

extern thread_local uint32									LThreadId;
extern thread_local	uint64									LEndTickCount;
extern thread_local std::shared_ptr<class SendBufferChunk>	LSendBufferChunk;
extern thread_local bool									LCanReturnSendBufferChunk;
extern thread_local class JobBaseObject*					LCurrentJobBaseObject;
//extern thread_local class ThreadCallStack					LThreadCallStack;