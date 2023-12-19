#include "pch.h"
#include "Session.h"
#include "SocketUtils.h"
#include "NetworkService.h"

Session::Session()
								//	TEMP
	: _recvBuffer(BUFFER_SIZE)/*, _sendBufferQueue(8192)*/
{
	_socket = SocketUtils::CreateSocket();
}

Session::~Session()
{
	SocketUtils::Close(_socket);
}

//	sendBuffer Ref = 2
//	Default Send Packet Only Push
void Session::Send(std::shared_ptr<SendBuffer> sendBuffer, bool onlyPush)
{
	if (IsConnected() == false)
		return;

	{
		WRITE_LOCK;

		_sendBufferQueue.push(std::move(sendBuffer));
	}

	if (onlyPush == false)
		RegisterSend();
}

void Session::FlushSend()
{
	RegisterSend();
}

bool Session::Connect()
{
	return RegisterConnect();
}

void Session::Disconnect(const WCHAR* cause)
{
	if (_isConnected.exchange(false) == false)
		return;

	std::wcout << "Disconnect : " << cause << std::endl;

	RegisterDisconnect();
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Session::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	switch (iocpEvent->GetEventType())
	{
	case EventType::Connect:
		ProcessConnect();
		break;
	case EventType::Disconnect:
		ProcessDisconnect();
		break;
	case EventType::Recv:
		ProcessRecv(numOfBytes);
		break;
	case EventType::Send:
		ProcessSend(numOfBytes);
		break;
	default:
		break;
	}
}

//	Server to Server
bool Session::RegisterConnect()
{
	if (_isConnected.load())
		return false;

	if (GetNetworkService()->GetServiceType() != ServiceType::Client)
		return false;

	_connectEvent.Initialize();
	_connectEvent.SetOwner(shared_from_this());

	if (SocketUtils::SetSocketReuseAddress(_socket, true) == false)
		return false;

	if (SocketUtils::BindAnyAddress(_socket, 0/*Any Port*/) == false)
		return false;

	DWORD numOfBytes = 0;
	//	Remote Address
	SOCKADDR_IN address = GetNetworkService()->GetNetworkAddress().GetAddress();
	if (false == SocketUtils::ConnectEx(_socket, reinterpret_cast<SOCKADDR*>(&address), sizeof(address),
		nullptr, 0, OUT & numOfBytes, &_connectEvent))
	{
		auto errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			ErrorHandling(errorCode);

			//	session Ref --
			_connectEvent.SetOwner(nullptr);
			return false;
		}
	}

	return true;
}

bool Session::RegisterDisconnect()
{
	_disconnectEvent.Initialize();
	_disconnectEvent.SetOwner(shared_from_this());

	if (false == SocketUtils::DisconnectEX(_socket, &_disconnectEvent, TF_REUSE_SOCKET, 0))
	{
		auto errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			ErrorHandling(errorCode);

			//	session Ref --
			_disconnectEvent.SetOwner(nullptr);
			return false;
		}
	}

	return true;
}

void Session::RegisterRecv()
{
	if (_isConnected.load() == false)
		return;

	_recvEvent.Initialize();
	//	session Ref ++
	_recvEvent.SetOwner(shared_from_this());

	DWORD numOfBytes = 0;
	DWORD flag = 0;

	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(_recvBuffer.WritePos());
	//	TEMP
	wsaBuf.len = _recvBuffer.RemainSize();

	if (SOCKET_ERROR == ::WSARecv(_socket, &wsaBuf, 1, OUT & numOfBytes, OUT & flag,
		&_recvEvent, nullptr))
	{
		auto errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			ErrorHandling(errorCode);

			//	session Ref --
			_recvEvent.SetOwner(nullptr);
		}
	}
}

//	Current One Thread Finish -> Next Thread 
//	Lock Used Thread Safe
//	Lock Free -> ????
void Session::RegisterSend()
{
	if (_isConnected.load() == false)
		return;

	//	보낼 작업이 없으면 스킵..
	{
		WRITE_LOCK;

		if (_sendBufferQueue.empty() == true)
			return;
	}

	if (_isRegisterdSend.exchange(true, std::memory_order_acquire) == true)
		return;

	_sendEvent.Initialize();
	//	session Ref ++
	_sendEvent.SetOwner(shared_from_this());

	{
		//	문제는 ... Pop 속도가 Push 속도를 쫓아가지 못하면 무한 루프다..
		//	Send 일감이 밀리면..	시간제한을 두고 할지 고민이다..
		int32 writeSize = 0;

		WRITE_LOCK;

		//	반응성을 포기하고 패킷 모아 보내기 시도할 수 있음..
		//	전투나 이동 같은 부분은 반응성이 중요하므로 제외..
		//	채팅 귓말 같은 것은 0.몇초 뒤에 와도 지장이 별로 없음
		//	그래서 채팅같이 반응성이 별로 중요하지 않은 것들은 따로 모아보내도 됨..
		//	물론 네트워크 코어단에서 해도되고 서버단에서 해도됨..
		while (_sendBufferQueue.empty() == false)
		{
			std::shared_ptr<SendBuffer> sendBuffer;
			sendBuffer = _sendBufferQueue.front();
			_sendBufferQueue.pop();

			//	TODO : Send Write Control
			writeSize += sendBuffer->WriteSize();

			_sendEvent.GetSendBuffers().push_back(std::move(sendBuffer));
		}
	}

	Vector<WSABUF> wsaBufs;
	wsaBufs.reserve(_sendEvent.GetSendBuffers().size());
	for (auto& sendBuffer : _sendEvent.GetSendBuffers())
	{
		WSABUF wsaBuf;
		wsaBuf.buf = reinterpret_cast<char*>(sendBuffer->Buffer());
		wsaBuf.len = static_cast<ULONG>(sendBuffer->WriteSize());
		wsaBufs.push_back(wsaBuf);
	}


	DWORD numOfBytes = 0;

	if (SOCKET_ERROR == ::WSASend(_socket, wsaBufs.data(), static_cast<DWORD>(wsaBufs.size()), OUT & numOfBytes, 0, &_sendEvent, nullptr))
	{
		auto errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			ErrorHandling(errorCode);

			//	session Ref --
			_sendEvent.SetOwner(nullptr);
			//	sendBuffers Ref --
			//	살짝 문제 있어 보이는데?
			//	아니 sendEvent sendBuffer는 session 공용인데
			//	한 스레드에서 WSASend 실패 했다고 공용으로 사용하는 Buffer를 지 마음대로 클리어?
			//	다른 스레드에서 쌓아온 sendBuffer 데이터는 못보내고 날라가는데 그러면?
			//	이미 복사를 해놓아서 상관이 없을 수도 있겠는데?
			//	TODO : 원격에 문제가 생겨서 연결이 끊긴 경우는 맞긴하다.
			//	근데 연결이 끊기지 않고 다른 문제로 인해서 여기로 왔으면 음... 애매한데
			_sendEvent.GetSendBuffers().clear();
			_isRegisterdSend.store(false, std::memory_order_release);
		}
	}
}

void Session::ProcessConnect()
{
	//	Client to Server or Server to Server
	_connectEvent.SetOwner(nullptr);

	_isConnected.store(true);

	//	NetworkService _sessions Ref ++
	GetNetworkService()->AddSession(GetSession());

	//	Conetents
	OnConnected();

	//	Recv Register
	RegisterRecv();
}

void Session::ProcessDisconnect()
{
	_disconnectEvent.SetOwner(nullptr);

	//	Content
	OnDisconnected();

	//	TEMP
	GetNetworkService()->RemoveSession(GetSession());
}

void Session::ProcessRecv(int32 numOfBytes)
{
	//	session Ref --
	_recvEvent.SetOwner(nullptr);

	if (numOfBytes == 0)
	{
		Disconnect(L"Recv 0");
		return;
	}

	if (_recvBuffer.OnWrite(numOfBytes) == false)
	{
		Disconnect(L"OnWrite Fail");
		return;
	}
	
	auto dataSize = _recvBuffer.DataSize();

	//	Contents --
	auto processLen = OnRecv(_recvBuffer.ReadPos(), dataSize);
	if (processLen < 0 || processLen > dataSize || 
		_recvBuffer.OnRead(processLen) == false)
	{
		Disconnect(L"OnRead Fail");
		return;
	}

	bool registerRecvAndProcessRecvTimingIssue = (_networkService.lock()->GetServiceType() == ServiceType::Client) ? true : false;
	_recvBuffer.Clear(registerRecvAndProcessRecvTimingIssue);

	RegisterRecv();
}

void Session::ProcessSend(int32 numOfBytes)
{
	//	문제가 보이긴한다..
	//	만약에 RegisterSend가 첫번째 WSASend를 하고 두번째 _sendEvent에 sendBuffer를 등록 시
	//  ProcessSend을 담당하는 워커스레드가 일감이 너무 밀려서 첫번째 WSASend를 처리하지 못해
	//	_sendEvent를 비우지못하다가 _sendEvent에 sendBuffer를 등록하는 스레드와 동일 타이밍에
	//	ProcessSend를 담당하는 스레드는 _sendEvent를 Clear 하고 RegisterSend 담당하는 스레드에서 _sendEvent에 sendBuffer 등록하면
	//	크래시난다..
	//	One Thread
	//	session Ref --

	{
		_sendEvent.SetOwner(nullptr);
		//	sendBuffers Ref --
		_sendEvent.GetSendBuffers().clear();
	}

	//	BoundedQueue .. Clear? 애매한데이거...
	if (numOfBytes == 0)
	{
		Disconnect(L"Send 0");
		return;
	}

	//	Contents --
	OnSend(numOfBytes);

	//	크래쉬..?
	if (_isRegisterdSend.exchange(false, std::memory_order_release) == false)
		return;

}

void Session::ErrorHandling(int32 errorCode)
{
	switch (errorCode)
	{
	case WSAECONNRESET:
	case WSAECONNABORTED:
		Disconnect(L"Error");
	default:
		//	TODO : Log Thread 
		std::cout << "Error : " << errorCode << std::endl;
		break;
	}
}

void Session::Clear()
{
	_recvEvent.SetOwner(nullptr);
	_sendEvent.SetOwner(nullptr);
	_connectEvent.SetOwner(nullptr);
	_disconnectEvent.SetOwner(nullptr);
}

PacketSession::PacketSession()
{

}

PacketSession::~PacketSession()
{

}

int32 PacketSession::OnRecv(BYTE* buffer, int32 len)
{
	//	One Packet Process Size
	int32 processLen = 0;
	while (true)
	{
		//	Total Recv Byte Len - One Packet Process Size -> Remain Recv Byte Len
		int32 dataSize = len - processLen;

		// if less than Packet Header Size
		if (dataSize < sizeof(PacketHeader))
			break;

		PacketHeader* packetHeader = reinterpret_cast<PacketHeader*>(&buffer[processLen]);
		
		// if less than One Packet Size
		if (dataSize < packetHeader->size)
			break;

		//	Content Used
		OnRecvPacket(&buffer[processLen], packetHeader->size);

		//	Add One Packet Process Size 
		processLen += packetHeader->size;
	}

	return processLen;
}
