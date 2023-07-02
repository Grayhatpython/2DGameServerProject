#pragma once

template<typename T>
class CallableBase;

template<typename Ret, typename... Args>
class CallableBase<Ret(Args...)>
{
public:
	virtual ~CallableBase() = default;
	virtual Ret Invoke(Args... args) abstract;
};

template<typename T, typename Type, typename MemberFunc>
class MemberFuncCallable;

//	TODO
template<typename Ret, typename... Args, typename Type, typename MemberFunc>
class MemberFuncCallable<Ret(Args...), Type, MemberFunc> : public CallableBase<Ret(Args...)>
{
public:
	MemberFuncCallable(Type* owner, MemberFunc memberFunc)
		: _owner(owner), _memberFunc(memberFunc)
	{

	}

public:
	virtual Ret Invoke(Args... args) override
	{
		return  (_owner->*_memberFunc)(std::forward<Args>(args)...);
	}

private:
	Type*		_owner = nullptr;
	MemberFunc	_memberFunc;
};