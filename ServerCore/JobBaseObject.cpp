#include "pch.h"
#include "JobBaseObject.h"
#include "DistributeJobBaseObject.h"

void JobBaseObject::Push(std::shared_ptr<Job> job, bool isOnlyPush, bool isDistribute)
{
	const int32 prevCount = _jobCount.fetch_add(1);
	_jobQueue.Push(job);

	if (prevCount == 0 && isDistribute == true)
	{
		if (LCurrentJobBaseObject == nullptr && isOnlyPush == false)
		{
			Execute();
		}
		else
		{
			GDistributeJobBaseObject->Push(shared_from_this());
		}
	}
}

void JobBaseObject::Execute()
{
	//	Only One JobBaseObject Execute
	LCurrentJobBaseObject = this;

	while (true)
	{
		Vector<std::shared_ptr<Job>> jobs;
		_jobQueue.PopAll(OUT jobs);

		const int32 processjobCount = static_cast<int32>(jobs.size());

		for (int32 i = 0; i < processjobCount; i++)
			jobs[i]->Execute();

		break;

		/*
		if (_jobCount.fetch_sub(processjobCount) == processjobCount)
		{
			LCurrentJobBaseObject = nullptr;
			break;
		}

		//	?? No
		const uint64 currentTick = ::GetTickCount64();
		if (currentTick >= LEndTickCount)
		{
			LCurrentJobBaseObject = nullptr;
			GDistributeJobBaseObject->Push(shared_from_this());
			break;
		}
		*/
	}
}

void JobBaseObject::KeepExecute()
{
	LCurrentJobBaseObject = this;

	while (true)
	{
		Vector<std::shared_ptr<Job>> jobs;
		_jobQueue.PopAll(OUT jobs);

		const int32 processjobCount = static_cast<int32>(jobs.size());

		for (int32 i = 0; i < processjobCount; i++)
			jobs[i]->Execute();

		if (_jobCount.fetch_sub(processjobCount) == processjobCount)
		{
			LCurrentJobBaseObject = nullptr;
			break;
		}
	}
}
