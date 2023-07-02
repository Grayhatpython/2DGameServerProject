#pragma once
#include "DBConnection.h"

template<int32 N>
struct FullBits
{
	enum
	{
		value = (1 << (N - 1)) | FullBits<N - 1>::value
	};
};

template<>
struct FullBits<1>
{
	enum
	{
		value = 1
	};
};

template<>
struct FullBits<0>
{
	enum
	{
		value = 0
	};
};

template<int32 ParamCount, int32 ColumnCount>
class DBHelper
{
public:
	DBHelper(DBConnection* dbConnection, const WCHAR* query)
		: _dbConntion(dbConnection), _query(query)
	{
		::memset(_paramIndex, 0, sizeof(_paramIndex));
		::memset(_columnIndex, 0, sizeof(_columnIndex));
		_paramFlag = 0;
		_columnFlag = 0;
		_dbConntion->UnBind();
	}
	~DBHelper()
	{
		_dbConntion = nullptr;
		_query = nullptr;
	}

public:
	bool Validate()
	{
		return _paramFlag == FullBits<ParamCount>::value && _columnFlag == FullBits<ColumnCount>::value;
	}

	bool Execute()
	{
		ASSERT(Validate());
		return _dbConntion->Execute(_query);
	}

	bool Fetch()
	{
		return _dbConntion->Fetch();
	}

public:
	template<typename T>
	void BindParam(int32 index, T& value)
	{
		_dbConntion->BindParam(index + 1, &value, &_paramIndex[index]);
		_paramFlag |= (1LL << index);
	}

	void BindParam(int32 index, const WCHAR* value)
	{
		_dbConntion->BindParam(index + 1, value, &_paramIndex[index]);
		_paramFlag |= (1LL << index);
	}

	template<typename T, int32 N>
	void BindParam(int32 index, T(&value)[N])
	{
		_dbConntion->BindParam(index + 1, (const BYTE*)value, sizeof(T) * N , &_paramIndex[index]);
		_paramFlag |= (1LL << index);
	}

	template<typename T>
	void BindParam(int32 index,T *value, int32 N)
	{
		_dbConntion->BindParam(index + 1, (const BYTE*)value, sizeof(T) * N, &_paramIndex[index]);
		_paramFlag |= (1LL << index);
	}

public:
	template<typename T>
	void BindCol(int32 index, T& value)
	{
		_dbConntion->BindCol(index + 1, &value, &_columnIndex[index]);
		_columnFlag |= (1LL << index);
	}

	template<int32 N>
	void BindCol(int32 index, WCHAR(&value)[N])
	{
		_dbConntion->BindCol(index + 1, value, N - 1, &_columnIndex[index]);
		_columnFlag |= (1LL << index);
	}

	void BindCol(int32 index, WCHAR* value, int32 len)
	{
		_dbConntion->BindCol(index + 1, value, len - 1, &_columnIndex[index]);
		_columnFlag |= (1LL << index);
	}

	template<typename T, int32 N>
	void BindCol(int32 index, T(&value)[N])
	{
		_dbConntion->BindCol(index + 1, value, sizeof(T) * N, &_columnIndex[index]);
		_columnFlag |= (1LL << index);
	}

protected:
	DBConnection*	_dbConntion = nullptr;
	const WCHAR*	_query = nullptr;
	SQLLEN			_paramIndex[ParamCount > 0 ? ParamCount : 1];
	SQLLEN			_columnIndex[ColumnCount > 0 ? ColumnCount : 1];

	uint64			_paramFlag = 0;
	uint64			_columnFlag = 0;
};

