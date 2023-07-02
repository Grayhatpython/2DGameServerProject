#pragma once

class SendBufferChunk;
class SendBuffer : public std::enable_shared_from_this<SendBuffer>
{
public:
	SendBuffer(std::shared_ptr<SendBufferChunk> ownerChunk, BYTE* buffer, int32 allocSize);
	~SendBuffer();

public:
	BYTE*	Buffer() { return _buffer; }
	int32	WriteSize() { return _writeSize; }
	void	Close(uint32 writeSize);

private:
	BYTE*								_buffer = nullptr;
	uint32								_allocSize = 0;
	uint32								_writeSize = 0;
	std::shared_ptr<SendBufferChunk>	_ownerChunk;
};


class SendBufferChunk : public std::enable_shared_from_this<SendBufferChunk>
{
	enum
	{
		SEND_BUFFER_CHUNK_SIZE = 0x10000
	};

public:
	SendBufferChunk();
	~SendBufferChunk();

public:
	void Initialize();
	std::shared_ptr<SendBuffer> Open(uint32 allocSize);
	void Close(uint32 writeSize);
	
public:
	bool	IsOpen() { return _isOpen; }
	uint32	RemainSize() { return static_cast<uint32>(_buffer.size()) - _usedSize; }
	BYTE*	Buffer() { return &_buffer[_usedSize]; }

private:
	bool									_isOpen = false;
	uint32									_usedSize = 0;
	Array<BYTE, SEND_BUFFER_CHUNK_SIZE>		_buffer{};
};

class SendBufferManager
{
public:
	SendBufferManager();
	~SendBufferManager();

public:
	std::shared_ptr<SendBuffer>			Open(uint32 size);

private:
	std::shared_ptr<SendBufferChunk>	Pop();
	void								Push(std::shared_ptr<SendBufferChunk> sendBufferChunk);

private:
	static	void						PushChunk(SendBufferChunk* sendBufferChunk);

private:
	USE_LOCK;
	Vector<std::shared_ptr<SendBufferChunk>>	_sendBufferChunks;
};