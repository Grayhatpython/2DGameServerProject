#include "pch.h"
#include "IocpCore.h"
#include "IocpEvent.h"
IocpCore::IocpCore()
{
	_iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, NULL);
	ASSERT(_iocpHandle != INVALID_HANDLE_VALUE);
}

IocpCore::~IocpCore()
{
	::CloseHandle(_iocpHandle);
}

bool IocpCore::Register(std::shared_ptr<IocpObject> iocpObject)
{
	//	Key -> Session
	return ::CreateIoCompletionPort(iocpObject->GetHandle(), _iocpHandle, NULL, NULL);
}

bool IocpCore::Dispatch(uint32 timeOut)
{
	DWORD numOfBytes = 0;
	ULONG_PTR key = 0;
	IocpEvent* iocpEvent = nullptr;

	//	Key -> Session , OVERLAPPED -> Event
	if (::GetQueuedCompletionStatus(_iocpHandle, OUT & numOfBytes,
		OUT & key,
		reinterpret_cast<LPOVERLAPPED*>(OUT & iocpEvent), timeOut))
	{
		std::shared_ptr<IocpObject> iocpObject = iocpEvent->GetOwner();
		iocpObject->Dispatch(iocpEvent, numOfBytes);
	}
	else
	{
		int32 errorCode = ::WSAGetLastError();
		switch (errorCode)
		{
		case WAIT_TIMEOUT:
			return false;
		default:
			/*
			std::shared_ptr<IocpObject> iocpObject = iocpEvent->GetOwner();
			iocpObject->Dispatch(iocpEvent, numOfBytes);
			*/
			break;
		}
	}

	return true;
}
