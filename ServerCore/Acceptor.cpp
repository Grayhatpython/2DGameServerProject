#include "pch.h"
#include "Acceptor.h"
#include "SocketUtils.h"
#include "IocpEvent.h"
#include "Session.h"
#include "NetworkService.h"

Acceptor::Acceptor()
{

}

Acceptor::~Acceptor()
{
	SocketUtils::Close(_socket);
}

bool Acceptor::Start(std::shared_ptr<ServerService> serverService)
{
	_serverService = serverService;
	if (_serverService == nullptr)
		return false;

	_socket = SocketUtils::CreateSocket();

	if (_socket == INVALID_SOCKET)
		return false;

	if (_serverService->GetIocpCore()->Register(shared_from_this()) == false)
		return false;

	if (SocketUtils::SetSocketReuseAddress(_socket, true) == false)
		return false;

	if (SocketUtils::SetSocketLinger(_socket, 0, 0) == false)
		return false;

	if (SocketUtils::Bind(_socket, _serverService->GetNetworkAddress()) == false)
		return false;

	if (SocketUtils::Listen(_socket) == false)
		return false;
	
	const int32 acceptCount = _serverService->GetMaxSessionCount();
	for (int32 i = 0; i < acceptCount; i++)
	{
		AcceptEvent* acceptEvent = cnew<AcceptEvent>();
		//	acceptCount 만큼 Acceptor의 Ref Count가 증가됨
		//	Acceptor 소멸자 호출 가능?
		acceptEvent->SetOwner(shared_from_this());
		_acceptEvents.push_back(acceptEvent);
		RegisterAccept(acceptEvent);
	}

	return true;
}

void Acceptor::Clear()
{
	for (auto acceptEvent : _acceptEvents)
		cdelete(acceptEvent);

	_acceptEvents.clear();
}

void Acceptor::Close()
{
	_serverService = nullptr;
	Clear();
}

HANDLE Acceptor::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Acceptor::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	ASSERT(iocpEvent->GetEventType() == EventType::Accept);

	AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(iocpEvent);

	ProcessAccept(acceptEvent);
}

void Acceptor::RegisterAccept(AcceptEvent* acceptEvent)
{
	//	Register Iocp
	auto session = _serverService->CreateSession();
	
	acceptEvent->Initialize();
	acceptEvent->SetSession(session);

	DWORD bytesReceived = 0;
	if (false == SocketUtils::AcceptEx(_socket, session->GetSocket(), session->_recvBuffer.WritePos(), 0,
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
		OUT & bytesReceived, static_cast<LPOVERLAPPED>(acceptEvent)))
	{
		const auto errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			//	펜딩 오류가 아닌 다른 오류 발생 시 다시 Accept 걸어준다
			RegisterAccept(acceptEvent);
		}
	}
}

void Acceptor::ProcessAccept(AcceptEvent* acceptEvent)
{
	auto session = acceptEvent->GetSession();

	if (SocketUtils::SetSocketUpdateAcceptSocket(session->GetSocket(), _socket) == false)
	{
		RegisterAccept(acceptEvent);
		return;
	}

	SOCKADDR_IN address;
	int32 addrSize = sizeof(address);
	if (SOCKET_ERROR == ::getpeername(session->GetSocket(), reinterpret_cast<SOCKADDR*>(&address), &addrSize))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	session->SetNetworkAddress(NetworkAddress(address));

	session->ProcessConnect();

	//	TODO
	RegisterAccept(acceptEvent);


}
