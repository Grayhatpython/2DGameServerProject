#include "pch.h"
#include "DistributeJobBaseObject.h"
#include "JobBaseObject.h"

DistributeJobBaseObject::DistributeJobBaseObject()
{

}

DistributeJobBaseObject::~DistributeJobBaseObject()
{

}

void DistributeJobBaseObject::Push(std::shared_ptr<JobBaseObject> jobBaseObject)
{
	_jobBaseObjectQueue.Push(jobBaseObject);
}

std::shared_ptr<JobBaseObject> DistributeJobBaseObject::Pop()
{
	return _jobBaseObjectQueue.Pop();
}

void DistributeJobBaseObject::Distribute()
{
	while (true)
	{
		auto currentTick = ::GetTickCount64();
		if (currentTick > LEndTickCount)
			break;

		auto jobBaseObject = GDistributeJobBaseObject->Pop();
		if (jobBaseObject == nullptr)
			return;

		jobBaseObject->Execute();
	}
}
