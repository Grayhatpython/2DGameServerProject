#pragma once

#include <vector>
#include <atomic>

template<typename T>
class BoundedQueue
{
public:
	explicit BoundedQueue(size_t size);
	BoundedQueue(BoundedQueue&& rhs) noexcept;
	BoundedQueue& operator=(BoundedQueue&& rhs) noexcept;

	bool	Push(T&& data);
	bool	Pop(T& data);
	bool	Pop();
	void	Clear();
	bool	Empty();

public:
	size_t	GetCount() { return _count.load(); }

private:
	struct Item
	{
		Item() = default;
		Item(Item&& rhs)
			: sequence(rhs.sequence.load()), data(std::move(rhs.data))
		{

		}

		Item& operator=(Item&& rhs)
		{
			sequence = rhs.sequence.load();
			data = std::move(rhs.data);
			return *this;
		}

		std::atomic<size_t> sequence;
		T data;
	};

private:
	using cacheline = char[64];

	cacheline _cachePadding1;
	std::vector<Item>	_queue;
	size_t				_queueMask = 0;
	cacheline _cachePadding2;
	std::atomic<size_t>	_enqueuePos = 0;
	cacheline _cachePadding3;
	std::atomic<size_t> _dequeuePos = 0;
	cacheline _cachePadding4;
	std::atomic<size_t>	_count = 0;
	cacheline _cachePadding5;
};

template<typename T>
inline BoundedQueue<T>::BoundedQueue(size_t size)
	: _queue(size), _queueMask(size - 1)
{
	for (size_t i = 0; i < size; i++)
		_queue[i].sequence = i;
}

template<typename T>
inline BoundedQueue<T>::BoundedQueue(BoundedQueue&& rhs) noexcept
{
	*this = rhs;
}

template<typename T>
inline  BoundedQueue<T>& BoundedQueue<T>::operator=(BoundedQueue&& rhs) noexcept
{
	if (this != &rhs)
	{
		_queue = std::move(rhs._queue);
		_queueMask = rhs._queueMask;
		_enqueuePos = rhs._enqueuePos.load();
		_dequeuePos = rhs._dequeuePos.load();
	}

	return *this;
}

template<typename T>
inline bool BoundedQueue<T>::Pop(T& data)
{
	Item* item = nullptr;
	auto pos = _dequeuePos.load(std::memory_order_relaxed);

	while (true)
	{
		item = &_queue[pos & _queueMask];
		auto sequence = item->sequence.load(std::memory_order_acquire);
		__int64 diff = static_cast<__int64>(sequence) - static_cast<__int64>(pos + 1);
		if (diff == 0)
		{
			if (_dequeuePos.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed))
				break;
		}
		else if (diff < 0)
			return false;
		else
			pos = _dequeuePos.load(std::memory_order_relaxed);
	}

	data = std::move(item->data);
	_count.fetch_sub(1, std::memory_order_relaxed);
	item->sequence.store(pos + _queueMask + 1, std::memory_order_release);
	return true;
}

template<typename T>
inline bool BoundedQueue<T>::Pop()
{
	Item* item = nullptr;
	auto pos = _dequeuePos.load(std::memory_order_relaxed);

	while (true)
	{
		item = &_queue[pos & _queueMask];
		auto sequence = item->sequence.load(std::memory_order_acquire);
		__int64 diff = static_cast<__int64>(sequence) - static_cast<__int64>(pos + 1);
		if (diff == 0)
		{
			if (_dequeuePos.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed))
				break;
		}
		else if (diff < 0)
			return false;
		else
			pos = _dequeuePos.load(std::memory_order_relaxed);
	}

	item->data = nullptr;
	_count.fetch_sub(1, std::memory_order_relaxed);
	item->sequence.store(pos + _queueMask + 1, std::memory_order_release);
	return true;
}

template<typename T>
inline void BoundedQueue<T>::Clear()
{
	while (true)
	{
		if (Pop() == false)
			break;
	}
}

template<typename T>
inline bool BoundedQueue<T>::Empty()
{
	if (_count.load(std::memory_order_relaxed) == 0)
		return true;

	return false;
}

template<typename T>
inline bool BoundedQueue<T>::Push(T&& data)
{
	Item* item = nullptr;
	auto pos = _enqueuePos.load(std::memory_order_relaxed);

	while (true)
	{
		item = &_queue[pos & _queueMask];
		auto sequence = item->sequence.load(std::memory_order_acquire);
		__int64 diff = static_cast<__int64>(sequence) - static_cast<__int64>(pos);
		if (diff == 0)
		{
			if (_enqueuePos.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed))
				break;
		}
		else if (diff < 0)
			return false;
		else
			pos = _enqueuePos.load(std::memory_order_relaxed);
	}

	item->data = std::move(data);
	_count.fetch_add(1, std::memory_order_relaxed);
	item->sequence.store(pos + 1, std::memory_order_release);
	return true;
}
