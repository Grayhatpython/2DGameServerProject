#include "pch.h"
#include "Allocator.h"

//	TEMP
void* StaticMemoryPoolAllocator::Allocate(int32 allocSize, const char* type)
{
	return GStaticMemoryPool->Allocate(allocSize, type);
}

void StaticMemoryPoolAllocator::Deallocate(void* ptr, const char* type)
{
	GStaticMemoryPool->Deallocate(ptr, type);
}
