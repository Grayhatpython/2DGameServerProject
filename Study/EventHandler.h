#pragma once

#include "Delegate.h"
#include "ConnectedEventHandler.h"

template<typename T>
class EventHandler;

template<typename Ret, typename... Args>
class EventHandler<Ret(Args...)>
{
	friend class ConnectedEventHandler;

public:
	template<typename T, typename MemberFunc>
	ConnectedEventHandler Bind(T* owner, MemberFunc memberFunc);

	void operator()(Args... args);
	void Invoke(Args... args);

private:
	void UnBind(CallableBase<Ret(Args...)>* callable);

private:
	std::vector<Delegate<Ret(Args...)>> _delegates;
};

template<typename Ret, typename... Args>
template<typename T, typename MemberFunc>
ConnectedEventHandler EventHandler<Ret(Args...)>::Bind(T* owner, MemberFunc memberFunc)
{
	Delegate<Ret(Args...)> delegate;
	delegate.Bind(owner, memberFunc);
	CallableBase<Ret(Args...)>* callable = delegate._callable;
	_delegates.push_back(std::move(delegate));
	return ConnectedEventHandler(this, callable);
}

template<typename Ret, typename... Args>
void EventHandler<Ret(Args...)>::operator()(Args... args)
{
	Invoke(std::forward<Args>(args)...);
}

template<typename Ret, typename... Args>
void EventHandler<Ret(Args...)>::Invoke(Args... args)
{
	for (auto& delegate : _delegates)
		delegate.Invoke(std::forward<Args>(args)...);
}

template<typename Ret, typename... Args>
void EventHandler<Ret(Args...)>::UnBind(CallableBase<Ret(Args...)>* callable)
{
	for (auto it = _delegates.begin(), end = _delegates.end(); it != end; ++it)
	{
		if (it->_callable == callable)
		{
			_delegates.erase(it);
			return;
		}
	}
}

#define EVENT_TYPE(NAME, ...)			using NAME = EventHandler<void(__VA_ARGS__)>
#define EVENT_TYPE_RET(NAME,RET, ...)	using NAME = EventHandler<RET(__VA_ARGS__)>