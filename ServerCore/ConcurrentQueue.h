#pragma once

template<typename T>
class ConcurrentQueue
{
public:
	void Push(T item)
	{
		WRITE_LOCK;
		_items.push(item);
	}

	T Pop()
	{
		WRITE_LOCK;
		if (_items.empty())
			return nullptr;

		T item = _items.front();
		_items.pop();

		return item;
	}

	void PopAll(OUT Vector<T>& items)
	{
		WRITE_LOCK;

		while (T item = Pop())
			items.push_back(item);
	}

	void Clear()
	{
		WRITE_LOCK;
		Queue<T>().swap(_items);
	}

private:
	USE_LOCK;
	Queue<T>	_items;
};

