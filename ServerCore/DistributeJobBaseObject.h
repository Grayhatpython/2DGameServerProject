#pragma once

class JobBaseObject;
class DistributeJobBaseObject
{
public:
	DistributeJobBaseObject();
	~DistributeJobBaseObject();

public:
	void							Push(std::shared_ptr<JobBaseObject> jobBaseObject);
	std::shared_ptr<JobBaseObject>	Pop();

public:
	static void						Distribute();

private:
	ConcurrentQueue<std::shared_ptr<JobBaseObject>> _jobBaseObjectQueue;
};

