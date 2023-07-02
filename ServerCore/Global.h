#pragma once

extern class ThreadManager*				GThreadManager;
extern class LockManager*				GLockManager;
extern class SendBufferManager*			GSendBufferManager;
extern class DistributeJobBaseObject*	GDistributeJobBaseObject;
extern class JobTimer*					GJobTimer;
extern class DBConnectionPool*			GDBConnectionPool;
extern class StaticMemoryPool*			GStaticMemoryPool;
extern class Logger*					GLogger;
class Global
{
public:
	Global();
	~Global();

public:
	void Initialize();
};

extern Global* GGlobal;