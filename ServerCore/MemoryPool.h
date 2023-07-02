#pragma once
#include "Allocator.h"
#include "Logger.h"

DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT)
struct MemoryHeader : public SLIST_ENTRY
{
	MemoryHeader(int32 allocSize)
		: allocSize(allocSize)
	{

	}

	static void* Attach(MemoryHeader* header, int32 size)
	{
		new(header)MemoryHeader(size);
		return reinterpret_cast<void*>(++header);
	}

	static MemoryHeader* Detach(void* ptr)
	{
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}
	
	int32 allocSize = 0;
};

//	x64 - 16 -> TEMP
DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT)
class MemoryPool
{
public:
	MemoryPool(int32 allocSize);
	~MemoryPool();

public:
	void Push(MemoryHeader* ptr, const  char* type);
	MemoryHeader* Pop(const  char* type);

private:
	SLIST_HEADER		_header;
	int32				_allocSize = 0;
	std::atomic<int32>	_useCount = 0;
	std::atomic<int32>	_reserveCount = 0;
};

class StaticMemoryPool
{
public:
	enum
	{
		MEMORY_POOL_COUNT = (1024 / 32) + (1024 / 128) + (2048 / 256),
		MAX_ALLOC_SIZE = 4096
	};

public:
	StaticMemoryPool();
	~StaticMemoryPool();

public:
	void*	Allocate(int32 size, const  char* type);
	void	Deallocate(void* ptr, const  char* type);

private:
	std::vector<MemoryPool*>					_memoryPools;
	std::array<MemoryPool*, MAX_ALLOC_SIZE + 1>	_memoryPoolTable;
};

template<typename T, typename... Args>
T* cnew(Args&&... args)
{
	T* memory = static_cast<T*>(StaticMemoryPoolAllocator::Allocate(sizeof(T), typeid(T).name()));
	new(memory)T(std::forward<Args>(args)...);
	return memory;
}

template<typename T>
void cdelete(T* obj)
{
	obj->~T();
	StaticMemoryPoolAllocator::Deallocate(obj, typeid(T).name());
}

template<typename T, typename... Args>
std::shared_ptr<T> MakeShared(Args&&... args)
{
	return std::shared_ptr<T>{ cnew<T>(std::forward<Args>(args)...), cdelete<T> };
}