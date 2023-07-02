#include "pch.h"
#include "SocketUtils.h"

LPFN_CONNECTEX		SocketUtils::ConnectEx = nullptr;
LPFN_DISCONNECTEX	SocketUtils::DisconnectEX = nullptr;
LPFN_ACCEPTEX		SocketUtils::AcceptEx = nullptr;

void SocketUtils::Initialize()
{
	WSADATA wsaData;

	ASSERT(::WSAStartup(MAKEWORD(2, 2), OUT &wsaData) == 0);

	SOCKET dummySocket = CreateSocket();
	ASSERT(BindSocketFunc(dummySocket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)));
	ASSERT(BindSocketFunc(dummySocket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEX)));
	ASSERT(BindSocketFunc(dummySocket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)));
}

void SocketUtils::Clear()
{
	::WSACleanup();
}

bool SocketUtils::BindSocketFunc(SOCKET socket, GUID guid, LPVOID* fn)
{
	DWORD bytes = 0;
	return SOCKET_ERROR != ::WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), OUT & bytes, NULL, NULL);
}

SOCKET SocketUtils::CreateSocket()
{
	return ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

bool SocketUtils::Bind(SOCKET socket, NetworkAddress networkAddress)
{
	return SOCKET_ERROR != ::bind(socket, reinterpret_cast<const SOCKADDR*>(&networkAddress.GetAddress()), sizeof(SOCKADDR_IN));
}

bool SocketUtils::BindAnyAddress(SOCKET socket, uint16 port)
{
	SOCKADDR_IN address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = ::htonl(INADDR_ANY);
	address.sin_port = ::htons(port);

	return SOCKET_ERROR != ::bind(socket, reinterpret_cast<const SOCKADDR*>(&address), sizeof(address));
}

bool SocketUtils::Listen(SOCKET socket, int32 backlog)
{
	return SOCKET_ERROR != ::listen(socket, backlog);
}

void SocketUtils::Close(SOCKET& socket)
{
	if (socket != INVALID_SOCKET)
	{
		::closesocket(socket);
		socket = INVALID_SOCKET;
	}
}

bool SocketUtils::SetSocketLinger(SOCKET socket, uint16 onoff, uint16 linger)
{
	LINGER option;
	option.l_onoff = onoff;
	option.l_linger = linger;
	return SetSocketOption(socket, SOL_SOCKET, SO_LINGER, option);
}

bool SocketUtils::SetSocketReuseAddress(SOCKET socket, bool flag)
{
	return SetSocketOption(socket, SOL_SOCKET, SO_REUSEADDR, flag);
}

bool SocketUtils::SetSocketRecvBufferSize(SOCKET socket, int32 size)
{
	return SetSocketOption(socket, SOL_SOCKET, SO_RCVBUF, size);
}

bool SocketUtils::SetSocketSendBufferSize(SOCKET socket, int32 size)
{
	return SetSocketOption(socket, SOL_SOCKET, SO_SNDBUF, size);
}

bool SocketUtils::SetSocketTcpNagle(SOCKET socket, bool flag)
{
	return SetSocketOption(socket, SOL_SOCKET, TCP_NODELAY, flag);
}

bool SocketUtils::SetSocketUpdateAcceptSocket(SOCKET socket, SOCKET acceptSocket)
{
	return SetSocketOption(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, acceptSocket);
}
