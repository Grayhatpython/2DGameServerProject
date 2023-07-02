//#pragma once
//
//template<int... Remains>
//struct seq
//{};
//
//template<int N, int... Remains>
//struct gen_seq : gen_seq<N - 1, N - 1, Remains...>
//{};
//
//template<int... Remains>
//struct gen_seq<0, Remains...> : seq<Remains...>
//{};
//
//template<typename Ret, typename... Args>
//void capply(Ret(*func)(Args...), std::tuple<Args...>& tup)
//{
//	capply_helper(func, gen_seq<sizeof...(Args)>(), tup);
//}
//
//template<typename F, typename... Args, int... ls>
//void capply_helper(F func, seq<ls...>, std::tuple<Args...>& tup)
//{
//	(func)(std::get<ls>(tup)...);
//}
//
//template<typename T, typename Ret, typename... Args>
//void capply(T* obj, Ret(T::* func)(Args...), std::tuple<Args...>& tup)
//{
//	capply_helper(obj, func, gen_seq<sizeof...(Args)>(), tup);
//}
//
//template<typename T, typename F, typename... Args, int... ls>
//void capply_helper(T* obj, F func, seq<ls...>, std::tuple<Args...>& tup)
//{
//	(obj->*func)(std::get<ls>(tup)...);
//}
//
//class IJob
//{
//public:
//	virtual void Execute() abstract;
//};
//
//template<typename Ret, typename... Args>
//class FuncJob : public IJob
//{
//	using FuncType = Ret(*)(Args...);
//
//public:
//	FuncJob(FuncType func, Args... args)
//		: _func(func), _args(args...)
//	{
//
//	}
//
//	virtual void Execute() override
//	{
//		capply(_func, _args);
//	}
//
//private:
//	FuncType			_func;
//	std::tuple<Args...> _args;
//};
//
//template<typename T, typename Ret, typename... Args>
//class MemberJob : public IJob
//{
//	using FuncType = Ret(T::*)(Args...);
//
//public:
//	MemberJob(T* obj, FuncType func, Args... args)
//		: _obj(obj), _func(func), _args(args...)
//	{
//
//	}
//
//	virtual void Execute() override
//	{
//		capply(_obj, _func, _args);
//	}
//
//private:
//	T* _obj;
//	FuncType			_func;
//	std::tuple<Args...> _args;
//};
//
//class JobQueue
//{
//public:
//	void Push(std::shared_ptr<IJob> job)
//	{
//		WriteLockGuard guard(_lock);
//		_jobs.push(job);
//	}
//
//	std::shared_ptr<IJob> Pop()
//	{
//		WriteLockGuard guard(_lock);
//		if (_jobs.empty())
//			return nullptr;
//
//		std::shared_ptr<IJob> job = _jobs.front();
//		_jobs.pop();
//
//		return job;
//	}
//
//private:
//	Lock							_lock;
//	queue<std::shared_ptr<IJob>>	_jobs;
//};
//
