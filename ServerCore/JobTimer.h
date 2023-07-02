#pragma once

class Job;
class JobBaseObject;
struct JobInfo
{
	JobInfo(std::weak_ptr<JobBaseObject> owner, std::shared_ptr<Job> job)
		: owner(owner), job(job)
	{

	}

	std::weak_ptr<JobBaseObject>	owner;
	std::shared_ptr<Job>			job;
};

struct JobTimerInfo
{
	bool operator<(const JobTimerInfo& other) const
	{
		return executeTick > other.executeTick;
	}

	uint64		executeTick = 0;
	JobInfo*	jobInfo = nullptr;
};

class JobTimer
{
public:
	~JobTimer();

public:
	std::shared_ptr<Job>	Reserve(uint64 tick, std::weak_ptr<JobBaseObject> owner, std::shared_ptr<Job> job);
	void					Execute(uint64 currentTick);
	void					Clear();

private:
	USE_LOCK;
	PriorityQueue<JobTimerInfo>	_jobs;
	std::atomic<bool>			_isExecuted = false;
};

