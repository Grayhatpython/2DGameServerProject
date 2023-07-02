#include "pch.h"
#include "RecvBuffer.h"
RecvBuffer::RecvBuffer(int32 bufferSize)
	: _bufferSize(bufferSize)
{
	_capacity = _bufferSize * MAX_BUFFER_COUNT;
	_buffer.resize(_capacity);
}

RecvBuffer::~RecvBuffer()
{

}

void RecvBuffer::Clear(bool registerRecvAndProcessRecvTimingIssue)
{
	int32 dataSize = DataSize();
	if (registerRecvAndProcessRecvTimingIssue == false && dataSize == 0)
	{
		_readPos = _writePos = 0;
	}
	else
	{
		//	if the remaining size is less than the buffer size, 
		//	the remaining data is copied to the beginning of the buffer.
		if (RemainSize() < _bufferSize)
		{
			::memcpy(&_buffer[0], &_buffer[_readPos], dataSize);
			_readPos = 0;
			_writePos = dataSize;
		}
	}
}

bool RecvBuffer::OnRead(int32 numOfBytes)
{
	if (numOfBytes > DataSize())
		return false;

	_readPos += numOfBytes;
	return true;
}

bool RecvBuffer::OnWrite(int32 numOfBytes)
{
	if (numOfBytes > RemainSize())
		return false;

	_writePos += numOfBytes;
	return true;
}
