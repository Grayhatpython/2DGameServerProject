#include "pch.h"
#include "Global.h"
#include "ThreadManager.h"
#include "LockManager.h"
#include "MemoryPool.h"
#include "SendBuffer.h"
#include "DistributeJobBaseObject.h"
#include "JobTimer.h"
#include "DBConnectionPool.h"
#include "Logger.h"
#include "SocketUtils.h"

ThreadManager*				GThreadManager = nullptr;
LockManager*				GLockManager = nullptr;
SendBufferManager*			GSendBufferManager = nullptr;
DistributeJobBaseObject*	GDistributeJobBaseObject = nullptr;
JobTimer*					GJobTimer = nullptr;
DBConnectionPool*			GDBConnectionPool = nullptr;
StaticMemoryPool*			GStaticMemoryPool = nullptr;
Logger*						GLogger = nullptr;

Global::Global()
{
	SocketUtils::Initialize();
}

Global::~Global()
{
	if (GDistributeJobBaseObject)
	{
		delete GDistributeJobBaseObject;
		GDistributeJobBaseObject = nullptr;
	}
	if (GJobTimer)
	{
		delete GJobTimer;
		GJobTimer = nullptr;
	}
	if (GDBConnectionPool)
	{
		delete GDBConnectionPool;
		GDBConnectionPool = nullptr;
	}
	if (GSendBufferManager)
	{
		delete GSendBufferManager;
		GSendBufferManager = nullptr;
	}
	if (GThreadManager)
	{
		delete GThreadManager;
		GThreadManager = nullptr;
	}
	if (GLockManager)
	{
		delete GLockManager;
		GLockManager = nullptr;
	}
	if (GLogger)
	{
		delete GLogger;
		GLogger = nullptr;
	}
	if (GStaticMemoryPool)
	{
		delete GStaticMemoryPool;
		GStaticMemoryPool = nullptr;
	}
}

void Global::Initialize()
{
	GThreadManager = new ThreadManager();
	GLockManager = new LockManager();
	GStaticMemoryPool = new StaticMemoryPool();
	GSendBufferManager = new SendBufferManager();
	GDistributeJobBaseObject = new DistributeJobBaseObject();
	GJobTimer = new JobTimer();
	GDBConnectionPool = new DBConnectionPool();
	GLogger = new Logger;
}

Global* GGlobal = nullptr;