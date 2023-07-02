#include "pch.h"
#include "SendBuffer.h"

SendBuffer::SendBuffer(std::shared_ptr<SendBufferChunk> ownerChunk, BYTE* buffer, int32 allocSize)
	:_ownerChunk(ownerChunk), _buffer(buffer), _allocSize(allocSize)
{

}

SendBuffer::~SendBuffer()
{

}

void SendBuffer::Close(uint32 writeSize)
{
	ASSERT(_allocSize >= writeSize);
	_writeSize = writeSize;
	_ownerChunk->Close(writeSize);
}

SendBufferChunk::SendBufferChunk()
{

}

SendBufferChunk::~SendBufferChunk()
{
	std::wcout << LThreadId << " ~SendBufferChunk" << std::endl;
}

void SendBufferChunk::Initialize()
{
	_isOpen = false;
	_usedSize = 0;
}

std::shared_ptr<SendBuffer> SendBufferChunk::Open(uint32 allocSize)
{
	ASSERT(allocSize <= SEND_BUFFER_CHUNK_SIZE);
	ASSERT(_isOpen == false);

	if (allocSize > RemainSize())
		return nullptr;

	_isOpen = true;

	return ObjectPool<SendBuffer>::MakeShared(shared_from_this(), Buffer(), allocSize);
}

void SendBufferChunk::Close(uint32 writeSize)
{
	ASSERT(_isOpen == true);
	_isOpen = false;
	_usedSize += writeSize;
}

SendBufferManager::SendBufferManager()
{

}

SendBufferManager::~SendBufferManager()
{

}

std::shared_ptr<SendBuffer> SendBufferManager::Open(uint32 size)
{
	//std::cout << "Thread [" << ::GetCurrentThreadId() << "] Send Buffer Chunk Pop" << std::endl;

	if (LSendBufferChunk == nullptr)
	{
		LSendBufferChunk = Pop();
		LSendBufferChunk->Initialize();
	}

	ASSERT(LSendBufferChunk->IsOpen() == false);

	if (LSendBufferChunk->RemainSize() < size)
	{
		//	Change 
		LSendBufferChunk = Pop();
		LSendBufferChunk->Initialize();
	}

	//std::cout << "Thread [" << ::GetCurrentThreadId() << "] Chunk Remain Size : " << LSendBufferChunk->RemainSize() << std::endl;
	return LSendBufferChunk->Open(size);
}

std::shared_ptr<SendBufferChunk> SendBufferManager::Pop()
{
	{
		WRITE_LOCK;
		if (_sendBufferChunks.empty() == false)
		{
			auto sendBufferChunk = _sendBufferChunks.back();
			_sendBufferChunks.pop_back();
			return sendBufferChunk;
		}
	}

	return std::shared_ptr<SendBufferChunk>(cnew<SendBufferChunk>(), PushChunk);
}

void SendBufferManager::Push(std::shared_ptr<SendBufferChunk> sendBufferChunk)
{
	WRITE_LOCK;

	_sendBufferChunks.push_back(sendBufferChunk);
}

void SendBufferManager::PushChunk(SendBufferChunk* sendBufferChunk)
{
	//std::cout << "Push SendBuffer Chunk" << std::endl;
	if (LCanReturnSendBufferChunk)
		GSendBufferManager->Push(std::shared_ptr<SendBufferChunk>(sendBufferChunk, PushChunk));
	else
		cdelete(sendBufferChunk);
}

