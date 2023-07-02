#include "pch.h"
#include "DBConnectionPool.h"

DBConnectionPool::DBConnectionPool()
{
}

DBConnectionPool::~DBConnectionPool()
{
	Clear();
}

bool DBConnectionPool::Initialize(int32 dbConnectionCount, const std::wstring& dbName, const WCHAR* dbConnectionString)
{
	WRITE_LOCK;

	if (::SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_enviroment) != SQL_SUCCESS)
		return false;

	if (::SQLSetEnvAttr(_enviroment, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0) != SQL_SUCCESS)
		return false;

	Vector<DBConnection*> dbConnects;

	for (int32 i = 0; i < dbConnectionCount; i++)
	{
		DBConnection* dbConnetion = cnew<DBConnection>();
		if (dbConnetion->Connect(_enviroment, dbConnectionString) == false)
		{
			cdelete(dbConnetion);
			return false;
		}

		dbConnects.push_back(dbConnetion);
	}
	
	_dbConnections.insert(std::make_pair(dbName, dbConnects));

	return true;
}

void DBConnectionPool::Clear()
{
	WRITE_LOCK;

	if (_enviroment != SQL_NULL_HANDLE)
	{
		::SQLFreeHandle(SQL_HANDLE_ENV, _enviroment);
		_enviroment = SQL_NULL_HANDLE;
	}

	for (auto dbConnection : _dbConnections)
	{
		for (auto db : dbConnection.second)
			cdelete(db);
	}

	_dbConnections.clear();
}

void DBConnectionPool::Push(const std::wstring& dbName, DBConnection* dbConnection)
{
	WRITE_LOCK;
	auto db = _dbConnections.find(dbName);
	if (db != _dbConnections.end())
		db->second.push_back(dbConnection);
	else
		ASSERT(false);
}

DBConnection* DBConnectionPool::Pop(const std::wstring& dbName)
{
	WRITE_LOCK;
	if (_dbConnections.empty())
		return nullptr;

	DBConnection* dbConnection = nullptr;
	auto db = _dbConnections.find(dbName);
	if (db != _dbConnections.end())
	{
		dbConnection = db->second.back();
		db->second.pop_back();
	}
	else
		ASSERT(false);

	return dbConnection;
}
