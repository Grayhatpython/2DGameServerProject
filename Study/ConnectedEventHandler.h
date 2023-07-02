#pragma once
#include <functional>

template<typename T>
class EventHandler;

template<typename T>
class CallableBase;

class ConnectedEventHandler
{
public:
	template<typename Ret, typename... Args>
	ConnectedEventHandler(EventHandler<Ret(Args...)>* eventHandler, CallableBase<Ret(Args...)>* callable)
		: _eventHandler(eventHandler), _callable(callable), _disconnectFunc(&DisconnectFunc<Ret, Args...>)
	{

	}

	void Disconnect()
	{
		if (_disconnectFunc)
			_disconnectFunc(_eventHandler, _callable);
	}

private:
	template<typename Ret, typename... Args>
	static void DisconnectFunc(void* signal, void* callable)
	{
		static_cast<EventHandler<Ret(Args...)>*>(signal)->UnBind(static_cast<CallableBase<Ret(Args...)>*>(callable));
	}

private:
	std::function<void(void*, void*)>	_disconnectFunc;
	void*								_eventHandler = nullptr;
	void*								_callable = nullptr;
};