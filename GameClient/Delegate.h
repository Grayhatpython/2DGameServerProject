#pragma once
#include "Callable.h"

template<typename T>
class EventHandler;

template<typename T>
class Delegate;

template<typename Ret, typename... Args>
class Delegate<Ret(Args...)>
{
	friend class EventHandler<Ret(Args...)>;

public:
	Delegate() = default;
	Delegate(Delegate&& other) noexcept;
	Delegate& operator=(Delegate&& other) noexcept;
	~Delegate();

public:
	template<typename T, typename MemberFunc>
	void Bind(T* owner, MemberFunc memberFunc);

	Ret operator()(Args... args) const;
	Ret Invoke(Args... args) const;

private:
	CallableBase<Ret(Args...)>* _callable = nullptr;
};

template<typename Ret, typename... Args>
Delegate<Ret(Args...)>::Delegate(Delegate&& other) noexcept
	: _callable(other._callable)
{
	other._callable = nullptr;
}

template<typename Ret, typename... Args>
Delegate<Ret(Args...)>& Delegate<Ret(Args...)>::operator=(Delegate&& other) noexcept
{
	Delegate temp(std::move(other));
	return *this;
}

template<typename Ret, typename... Args>
Delegate<Ret(Args...)>::~Delegate()
{
	delete _callable;
	_callable = nullptr;
}

template<typename Ret, typename... Args>
template<typename T, typename MemberFunc>
void Delegate<Ret(Args...)>::Bind(T* owner, MemberFunc memberFunc)
{
	_callable = new MemberFuncCallable<Ret(Args...), T, MemberFunc>(owner, memberFunc);
}

template<typename Ret, typename... Args>
Ret Delegate<Ret(Args...)>::operator()(Args... args) const
{
	return _callable->Invoke(std::forward<Args>(args)...);
}

template<typename Ret, typename... Args>
Ret  Delegate<Ret(Args...)>::Invoke(Args... args) const
{
	return _callable->Invoke(std::forward<Args>(args)...);
}