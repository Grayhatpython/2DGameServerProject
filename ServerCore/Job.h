#pragma once

using FuncType = std::function<void()>;

class Job
{
public:
	Job(std::function<void()>&& func)
		: _func(std::move(func))
	{

	}

	template<typename T, typename Ret, typename... Args>
	Job(std::shared_ptr<T> owner, Ret(T::* memberFunc)(Args...), Args&&... args)
	{
		_func = [owner, memberFunc, args...]()
		{
			(owner.get()->*memberFunc)(args...);
		};
	}

public:
	void Execute()
	{
		if(_cancel == false)
			_func();
	}

public:
	void		SetCancel(bool cancel) { _cancel = cancel; }

private:
	FuncType	_func;
	bool		_cancel = false;
};

