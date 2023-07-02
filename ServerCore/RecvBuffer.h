#pragma once

class RecvBuffer
{
	enum
	{
		MAX_BUFFER_COUNT = 10,
	};

public:
	RecvBuffer(int32 bufferSize);
	~RecvBuffer();

public:
	//	TODO
	void	Clear(bool registerRecvAndProcessRecvTimingIssue = false);
	bool	OnRead(int32 numOfBytes);
	bool	OnWrite(int32 numOfBytes);

public:
	BYTE*	ReadPos() { return &_buffer[_readPos]; }
	BYTE*	WritePos() { return &_buffer[_writePos]; }
	int32	DataSize() { return _writePos - _readPos; }
	int32	RemainSize() { return _capacity - _writePos; }

	int32	GetReadPos() const { return _readPos; }

private:
	int32			_capacity = 0;
	int32			_bufferSize = 0;
	int32			_readPos = 0;
	int32			_writePos = 0;
	Vector<BYTE>	_buffer;
};

