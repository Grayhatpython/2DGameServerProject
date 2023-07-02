#include "pch.h"
#include "JobTimer.h"
#include "JobBaseObject.h"

JobTimer::~JobTimer()
{
	//	TEMP
	Clear();
}

std::shared_ptr<Job> JobTimer::Reserve(uint64 tick, std::weak_ptr<JobBaseObject> owner, std::shared_ptr<Job> job)
{
	const auto executeTick = ::GetTickCount64() + tick;
	JobInfo* jobInfo = ObjectPool<JobInfo>::Pop(owner, job);

	{
		WRITE_LOCK;
		_jobs.push(JobTimerInfo{ executeTick, jobInfo });
	}

	return job;
}

void JobTimer::Execute(uint64 currentTick)
{
	if (_isExecuted.exchange(true) == true)
		return;

	Vector<JobTimerInfo> jobs;

	{
		WRITE_LOCK;
		while (_jobs.empty() == false)
		{
			const auto& job = _jobs.top();
			if (currentTick < job.executeTick)
				break;

			jobs.push_back(job);
			_jobs.pop();
		}
	}

	for (auto& job : jobs)
	{
		if (auto owner = job.jobInfo->owner.lock())
			owner->Push(job.jobInfo->job, true);

		ObjectPool<JobInfo>::Push(job.jobInfo);
	}

	_isExecuted.store(false);
}

void JobTimer::Clear()
{
	WRITE_LOCK;

	while (_jobs.empty() == false)
	{
		const auto& job = _jobs.top();
		ObjectPool<JobInfo>::Push(job.jobInfo);
		_jobs.pop();
	}
}
