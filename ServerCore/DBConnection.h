#pragma once
#include <sql.h>
#include <sqlext.h>

class DBConnection
{
	enum
	{
		WVARCHAR_MAX_SIZE = 4000,
		BINARY_MAX_SIZE = 8000
	};

public:
	DBConnection();
	~DBConnection();

public:
	bool Connect(SQLHENV enviroment, const WCHAR* dbConnectionString);
	void Clear();

public:
	bool Execute(const WCHAR* query);
	bool Fetch();
	void UnBind();
	int32 GetRowCount();

public:
	bool	BindParam(int32 paramIndex, bool* value, SQLLEN* index);
	bool	BindParam(int32 paramIndex, float* value, SQLLEN* index);
	bool	BindParam(int32 paramIndex, double* value, SQLLEN* index);
	bool	BindParam(int32 paramIndex, int8* value, SQLLEN* index);
	bool	BindParam(int32 paramIndex, int16* value, SQLLEN* index);
	bool	BindParam(int32 paramIndex, int32* value, SQLLEN* index);
	bool	BindParam(int32 paramIndex, int64* value, SQLLEN* index);
	bool	BindParam(int32 paramIndex, TIMESTAMP_STRUCT* value, SQLLEN* index);
	bool	BindParam(int32 paramIndex, const WCHAR* str, SQLLEN* index);
	bool	BindParam(int32 paramIndex, const BYTE* binary, int32 size, SQLLEN* index);

public:
	bool	BindCol(int32 paramIndex, bool* value, SQLLEN* index);
	bool	BindCol(int32 paramIndex, float* value, SQLLEN* index);
	bool	BindCol(int32 paramIndex, double* value, SQLLEN* index);
	bool	BindCol(int32 paramIndex, int8* value, SQLLEN* index);
	bool	BindCol(int32 paramIndex, int16* value, SQLLEN* index);
	bool	BindCol(int32 paramIndex, int32* value, SQLLEN* index);
	bool	BindCol(int32 paramIndex, int64* value, SQLLEN* index);
	bool	BindCol(int32 paramIndex, TIMESTAMP_STRUCT* value, SQLLEN* index);
	bool	BindCol(int32 paramIndex, WCHAR* str, int32 size, SQLLEN* index);
	bool	BindCol(int32 paramIndex, BYTE* binary, int32 size, SQLLEN* index);

private:
	bool	BindParam(SQLUSMALLINT paramIndex, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN len, SQLPOINTER ptr, SQLLEN* index);
	bool	BindCol(SQLUSMALLINT columnIndex, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* index);
	void	ErrorHandling(SQLRETURN ret);

private:
	SQLHDBC		_dbConnection = SQL_NULL_HANDLE;
	SQLHSTMT	_statement = SQL_NULL_HANDLE;
};

