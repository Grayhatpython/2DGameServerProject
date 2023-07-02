#pragma once
#include <memory>
#include "Types.h"
#include "MemoryPool.h"

template<typename T>
class ObjectPool
{
public:
	template<typename... Args>
	static T* Pop(Args&&... args)
	{
		T* memory = static_cast<T*>(MemoryHeader::Attach(s_memoryPool.Pop(typeid(T).name()), s_allocSize));

		new(memory)T(std::forward<Args>(args)...);
		return memory;
	}

	static void Push(T* obj)
	{
		obj->~T();

		s_memoryPool.Push(MemoryHeader::Detach(obj), typeid(T).name());
	}

	template<typename... Args>
	static std::shared_ptr<T> MakeShared(Args&&... args)
	{
		std::shared_ptr<T> ptr = { Pop(std::forward<Args>(args)...), Push };
		return ptr;
	}

private:
	static int32		s_allocSize;
	static MemoryPool	s_memoryPool;
};

template<typename T>
int32 ObjectPool<T>::s_allocSize = sizeof(T) + sizeof(MemoryHeader);

template<typename T>
MemoryPool ObjectPool<T>::s_memoryPool{ s_allocSize };
