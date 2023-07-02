#pragma once
#include "DBConnection.h"

class DBConnectionPool
{
public:
	DBConnectionPool();
	~DBConnectionPool();

public:
	bool			Initialize(int32 dbConnectionCount, const std::wstring& dbName, const WCHAR* dbConnectionString);
	void			Clear();

public:
	void			Push(const std::wstring& dbName, DBConnection* dbConnection);
	DBConnection*	Pop(const std::wstring& dbName);

private:
	USE_LOCK;
	SQLHENV											_enviroment = SQL_NULL_HANDLE;
	HashMap<std::wstring,Vector<DBConnection*>>		_dbConnections;
};

