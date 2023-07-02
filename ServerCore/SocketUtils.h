#pragma once

#include "NetworkAddress.h"
class SocketUtils
{
public:
	static void Initialize();
	static void Clear();

public:
	static bool		BindSocketFunc(SOCKET socket, GUID guid, LPVOID* fn);
	static SOCKET	CreateSocket();

public:
	static bool Bind(SOCKET socket, NetworkAddress networkAddress);
	static bool BindAnyAddress(SOCKET socket, uint16 port);
	static bool Listen(SOCKET socket, int32 backlog = SOMAXCONN);
	static void Close(SOCKET& socket);

public:
	static bool SetSocketLinger(SOCKET socket, uint16 onoff, uint16 linger);
	static bool SetSocketReuseAddress(SOCKET socket, bool flag);
	static bool SetSocketRecvBufferSize(SOCKET socket, int32 size);
	static bool SetSocketSendBufferSize(SOCKET socket, int32 size);
	static bool SetSocketTcpNagle(SOCKET socket, bool flag);
	static bool SetSocketUpdateAcceptSocket(SOCKET socket, SOCKET acceptSocket);

public:
	static LPFN_CONNECTEX		ConnectEx;
	static LPFN_DISCONNECTEX	DisconnectEX;
	static LPFN_ACCEPTEX		AcceptEx;

};

template<typename T>
static inline bool SetSocketOption(SOCKET socket, int32 level, int32 optionName, T optionValue)
{
	return SOCKET_ERROR != ::setsockopt(socket, level, optionName, reinterpret_cast<char*>(&optionValue), sizeof(T));
}