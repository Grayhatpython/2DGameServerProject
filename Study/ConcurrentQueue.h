#pragma once

template<typename T>
class ConcurrentQueue
{
public:
	void Push(T item)
	{
		//std::lock_guard<std::mutex> g(m);

		//::AcquireSRWLockExclusive(&s);
		WRITE_LOCK;
		_items.push(item);
		//::ReleasekSRWLockExclusive(&s);
	}

	T Pop()
	{
		//std::lock_guard<std::mutex> g(m);

		WRITE_LOCK;
		{
			//WRITE_LOCK;
		}
		//::AcquireSRWLockExclusive(&s);
		//::AcquireSRWLockExclusive(&s);
		if (_items.empty())
		{
			//::ReleaseSRWLockExclusive(&s);
			return -1;
		}

		T item = _items.front();
		_items.pop();

		//::ReleaseSRWLockExclusive(&s);

		return item;
	}

	void Clear()
	{

		//std::lock_guard<std::mutex> g(m);
		WRITE_LOCK;
		std::queue<T>().swap(_items);
	}

private:
	//SRWLOCK s;
	//std::mutex m;
	USE_LOCK;
	std::queue<T>	_items;
};

