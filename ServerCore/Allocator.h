#pragma once

class StaticMemoryPoolAllocator
{
public:
	static void*	Allocate(int32 allocSize, const char* type);
	static void		Deallocate(void* ptr, const char* type);
};

template<typename T>
class StlAllocator
{
public:
	using value_type = T;

	StlAllocator() { }

	template<typename Other>
	StlAllocator(const StlAllocator<Other>&) { }

	T* allocate(size_t count)
	{
		const int32 size = static_cast<int32>(count * sizeof(T));
		return static_cast<T*>(StaticMemoryPoolAllocator::Allocate(size, typeid(T).name()));
	}

	void deallocate(T* ptr, size_t count)
	{
		StaticMemoryPoolAllocator::Deallocate(ptr, typeid(T).name());
	}

	template<typename U>
	bool operator==(const StlAllocator<U>&) { return true; }

	template<typename U>
	bool operator!=(const StlAllocator<U>&) { return false; }
};