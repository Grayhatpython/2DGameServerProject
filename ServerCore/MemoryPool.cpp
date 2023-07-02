#include "pch.h"
#include "MemoryPool.h"

MemoryPool::MemoryPool(int32 allocSize)
	: _allocSize(allocSize)
{
	::InitializeSListHead(&_header);
}

MemoryPool::~MemoryPool()
{
	while (true)
	{
		MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header));
		if (memory)
			::_aligned_free(memory);
		else
			break;
	}
}

void MemoryPool::Push(MemoryHeader* ptr, const char* type)
{
	auto allocSize = ptr->allocSize;
	ptr->allocSize = 0;

	::InterlockedPushEntrySList(&_header, static_cast<PSLIST_ENTRY>(ptr));

	auto useCount = _useCount.fetch_sub(1);
	auto reserveCount =_reserveCount.fetch_add(1);

		//GLogger->Log("MemoryPool Push -> Type [%s] | Address [%p] | Real AllocSize [%d] | ReserveCount [%d] | UseCount [%d]", type, ptr, allocSize, reserveCount + 1, useCount - 1);
}

MemoryHeader* MemoryPool::Pop(const char* type)
{
	MemoryHeader* memory = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header));

	if (memory == nullptr)
	{
		memory = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(_allocSize, MEMORY_ALLOCATION_ALIGNMENT));
	}
	else
	{
		//	Memory Already Use ? Why
		assert(memory->allocSize == 0);
		_reserveCount.fetch_sub(1);
	}

	auto reserveCount = _reserveCount.load();
	auto useCount = _useCount.fetch_add(1);

		//GLogger->Log("MemoryPool Pop -> Type [%s] | Address [%p] | MemoryPool Table Mapping Index [%d] | ReserveCount [%d] | UseCount [%d]", type, memory, _allocSize, reserveCount, useCount + 1);
	return memory;
}

StaticMemoryPool::StaticMemoryPool()
{
	int32 tableIndex = 0;

	for (int32 size = 0; size <= 1024; size += 32)
	{
		MemoryPool* memoryPool = new MemoryPool(size);
		_memoryPools.push_back(memoryPool);

		while (tableIndex <= size)
		{
			_memoryPoolTable[tableIndex] = memoryPool;
			tableIndex++;
		}
	}

	for (int32 size = 1024; size <= 2048; size += 128)
	{
		MemoryPool* memoryPool = new MemoryPool(size);
		_memoryPools.push_back(memoryPool);

		while (tableIndex <= size)
		{
			_memoryPoolTable[tableIndex] = memoryPool;
			tableIndex++;
		}
	}

	for (int32 size = 2048; size <= MAX_ALLOC_SIZE; size += 256)
	{
		MemoryPool* memoryPool = new MemoryPool(size);
		_memoryPools.push_back(memoryPool);

		while (tableIndex <= size)
		{
			_memoryPoolTable[tableIndex] = memoryPool;
			tableIndex++;
		}
	}
}

StaticMemoryPool::~StaticMemoryPool()
{
	for (MemoryPool* memoryPool : _memoryPools)
		delete memoryPool;

	_memoryPools.clear();
}

void* StaticMemoryPool::Allocate(int32 size, const char* type)
{
	MemoryHeader* header = nullptr;
	const int32 allocSize = size + sizeof(MemoryHeader);

	if (allocSize > MAX_ALLOC_SIZE)
		header = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(allocSize, MEMORY_ALLOCATION_ALIGNMENT));
	else
		header = _memoryPoolTable[allocSize]->Pop(type);

	return MemoryHeader::Attach(header, allocSize);
}

void StaticMemoryPool::Deallocate(void* ptr, const char* type)
{
	MemoryHeader* header = MemoryHeader::Detach(ptr);

	const int32 allocSize = header->allocSize;
	ASSERT(allocSize > 0);

	if (allocSize > MAX_ALLOC_SIZE)
		::_aligned_free(header);
	else
		_memoryPoolTable[allocSize]->Push(header, type);
}
