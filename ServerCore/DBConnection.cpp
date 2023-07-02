#include "pch.h"
#include "DBConnection.h"

DBConnection::DBConnection()
{

}

DBConnection::~DBConnection()
{
	Clear();
}

bool DBConnection::Connect(SQLHENV enviroment, const WCHAR* dbConnectionString)
{
	if (::SQLAllocHandle(SQL_HANDLE_DBC, enviroment, &_dbConnection) != SQL_SUCCESS)
		return false;

	WCHAR connectionString[MAX_PATH] = { 0, };
	::wcscpy_s(connectionString, dbConnectionString);

	WCHAR resultStr[MAX_PATH] = { 0, };
	SQLSMALLINT resultStrLen = 0;

	auto ret = ::SQLDriverConnectW(_dbConnection, NULL, 
		reinterpret_cast<SQLWCHAR*>(connectionString), _countof(connectionString),
		reinterpret_cast<SQLWCHAR*>(resultStr), _countof(resultStr), OUT & resultStrLen, 
		SQL_DRIVER_NOPROMPT);

	if (::SQLAllocHandle(SQL_HANDLE_STMT, _dbConnection, &_statement) != SQL_SUCCESS)
		return false;

	return (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
}

void DBConnection::Clear()
{
	if (_dbConnection != SQL_NULL_HANDLE)
	{
		::SQLFreeHandle(SQL_HANDLE_DBC, _dbConnection);
		_dbConnection = SQL_NULL_HANDLE;
	}

	if (_statement != SQL_NULL_HANDLE)
	{
		::SQLFreeHandle(SQL_HANDLE_STMT, _statement);
		_statement = SQL_NULL_HANDLE;
	}
}

bool DBConnection::Execute(const WCHAR* query)
{
	auto ret = ::SQLExecDirectW(_statement, const_cast<SQLWCHAR*>(query), SQL_NTSL);
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
		return true;
	
	ErrorHandling(ret);
	return false;
}

bool DBConnection::Fetch()
{
	SQLRETURN ret = ::SQLFetch(_statement);

	switch (ret)
	{
	case SQL_SUCCESS:
	case SQL_SUCCESS_WITH_INFO:
		return true;
	case SQL_NO_DATA:
		return false;
	case SQL_ERROR:
		ErrorHandling(ret);
		return false;
	}

	return true;
}

//	StateMent Initialize
void DBConnection::UnBind()
{
	::SQLFreeStmt(_statement, SQL_UNBIND);
	::SQLFreeStmt(_statement, SQL_RESET_PARAMS);
	::SQLFreeStmt(_statement, SQL_CLOSE);
}

int32 DBConnection::GetRowCount()
{
	SQLLEN rowCount = 0;
	SQLRETURN ret = ::SQLRowCount(_statement, OUT &rowCount);
	
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
		return static_cast<int32>(rowCount);

	return -1;
}

bool DBConnection::BindParam(int32 paramIndex, bool* value, SQLLEN* index)
{
	return BindParam(paramIndex, SQL_C_TINYINT, SQL_TINYINT, sizeof(bool), value, index);
}

bool DBConnection::BindParam(int32 paramIndex, float* value, SQLLEN* index)
{
	return BindParam(paramIndex, SQL_C_FLOAT, SQL_REAL, 0, value, index);
}

bool DBConnection::BindParam(int32 paramIndex, double* value, SQLLEN* index)
{
	return BindParam(paramIndex, SQL_C_DOUBLE, SQL_DOUBLE, 0, value, index);
}

bool DBConnection::BindParam(int32 paramIndex, int8* value, SQLLEN* index)
{
	return BindParam(paramIndex, SQL_C_TINYINT, SQL_TINYINT, sizeof(int8), value, index);
}

bool DBConnection::BindParam(int32 paramIndex, int16* value, SQLLEN* index)
{
	return BindParam(paramIndex, SQL_C_SHORT, SQL_SMALLINT, sizeof(int16), value, index);
}

bool DBConnection::BindParam(int32 paramIndex, int32* value, SQLLEN* index)
{
	return BindParam(paramIndex, SQL_C_LONG, SQL_INTEGER, sizeof(int32), value, index);
}

bool DBConnection::BindParam(int32 paramIndex, int64* value, SQLLEN* index)
{
	return BindParam(paramIndex, SQL_C_SBIGINT, SQL_BIGINT, sizeof(int64), value, index);
}

bool DBConnection::BindParam(int32 paramIndex, TIMESTAMP_STRUCT* value, SQLLEN* index)
{
	return BindParam(paramIndex, SQL_C_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP, sizeof(TIMESTAMP_STRUCT), value, index);
}

bool DBConnection::BindParam(int32 paramIndex, const WCHAR* str, SQLLEN* index)
{
	SQLULEN size = static_cast<SQLULEN>((::wcslen(str) + 1) * 2);
	*index = SQL_NTSL;

	if(size > WVARCHAR_MAX_SIZE)
		return BindParam(paramIndex, SQL_C_WCHAR, SQL_WLONGVARCHAR, size, (SQLPOINTER)(str), index);
	else
		return BindParam(paramIndex, SQL_C_WCHAR, SQL_WVARCHAR, size, (SQLPOINTER)(str), index);
}

bool DBConnection::BindParam(int32 paramIndex, const BYTE* binary, int32 size, SQLLEN* index)
{
	if (binary == nullptr)
	{
		*index = SQL_NULL_DATA;
		size = 1;
	}
	else
		*index = size;

	if(size > BINARY_MAX_SIZE)
		return BindParam(paramIndex, SQL_C_BINARY, SQL_LONGVARBINARY, size, (BYTE*)(binary), index);
	else
		return BindParam(paramIndex, SQL_C_BINARY, SQL_BINARY, size, (BYTE*)(binary), index);
}

bool DBConnection::BindCol(int32 paramIndex, bool* value, SQLLEN* index)
{
	return BindCol(paramIndex, SQL_C_TINYINT, sizeof(bool), value, index);
}

bool DBConnection::BindCol(int32 paramIndex, float* value, SQLLEN* index)
{
	return BindCol(paramIndex, SQL_C_FLOAT, sizeof(float), value, index);
}

bool DBConnection::BindCol(int32 paramIndex, double* value, SQLLEN* index)
{
	return BindCol(paramIndex, SQL_C_DOUBLE, sizeof(double), value, index);
}

bool DBConnection::BindCol(int32 paramIndex, int8* value, SQLLEN* index)
{
	return BindCol(paramIndex, SQL_C_TINYINT, sizeof(int8), value, index);
}

bool DBConnection::BindCol(int32 paramIndex, int16* value, SQLLEN* index)
{
	return BindCol(paramIndex, SQL_C_SHORT, sizeof(int16), value, index);
}

bool DBConnection::BindCol(int32 paramIndex, int32* value, SQLLEN* index)
{
	return BindCol(paramIndex, SQL_C_LONG, sizeof(int32), value, index);
}

bool DBConnection::BindCol(int32 paramIndex, int64* value, SQLLEN* index)
{
	return BindCol(paramIndex, SQL_C_SBIGINT, sizeof(int64), value, index);
}

bool DBConnection::BindCol(int32 paramIndex, TIMESTAMP_STRUCT* value, SQLLEN* index)
{
	return BindCol(paramIndex, SQL_C_TYPE_TIMESTAMP, sizeof(TIMESTAMP_STRUCT), value, index);
}

bool DBConnection::BindCol(int32 paramIndex, WCHAR* str, int32 size, SQLLEN* index)
{
	return BindCol(paramIndex, SQL_C_WCHAR, size, str, index);
}

bool DBConnection::BindCol(int32 paramIndex, BYTE* binary, int32 size, SQLLEN* index)
{
	return BindCol(paramIndex, SQL_BINARY, size, binary, index);
}

bool DBConnection::BindParam(SQLUSMALLINT paramIndex, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN len, SQLPOINTER ptr, SQLLEN* index)
{
	auto ret = ::SQLBindParameter(_statement, paramIndex, SQL_PARAM_INPUT, cType, sqlType, len, 0, ptr, 0, index);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ErrorHandling(ret);
		return false;
	}

	return true;
}

bool DBConnection::BindCol(SQLUSMALLINT columnIndex, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* index)
{
	auto ret = ::SQLBindCol(_statement, columnIndex, cType, value, len, index);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ErrorHandling(ret);
		return false;
	}

	return true;
}

void DBConnection::ErrorHandling(SQLRETURN ret)
{
	if (ret == SQL_SUCCESS)
		return;

	SQLSMALLINT index = 1;
	SQLWCHAR sqlState[MAX_PATH] = { 0, };
	SQLINTEGER nativeError = 0;
	SQLWCHAR errorMsg[MAX_PATH] = { 0, };
	SQLSMALLINT msgLen = 0;
	SQLRETURN errorRet = 0;

	while (true)
	{
		errorRet = ::SQLGetDiagRecW(
			SQL_HANDLE_STMT,
			_statement,
			index,
			sqlState,
			OUT & nativeError,
			errorMsg,
			_countof(errorMsg),
			OUT & msgLen
		);

		if (errorRet == SQL_NO_DATA)
			break;

		if (errorRet != SQL_SUCCESS && errorRet != SQL_SUCCESS_WITH_INFO)
			break;

		wcout.imbue(std::locale("kor"));
		std::wcout << errorMsg << std::endl;

		index++;
	}
}
