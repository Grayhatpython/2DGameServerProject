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

	//	���� �۾��� ������ ��ŵ..
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
		//	������ ... Pop �ӵ��� Push �ӵ��� �Ѿư��� ���ϸ� ���� ������..
		//	Send �ϰ��� �и���..	�ð������� �ΰ� ���� ����̴�..
		int32 writeSize = 0;

		WRITE_LOCK;

		//	�������� �����ϰ� ��Ŷ ��� ������ �õ��� �� ����..
		//	������ �̵� ���� �κ��� �������� �߿��ϹǷ� ����..
		//	ä�� �Ӹ� ���� ���� 0.���� �ڿ� �͵� ������ ���� ����
		//	�׷��� ä�ð��� �������� ���� �߿����� ���� �͵��� ���� ��ƺ����� ��..
		//	���� ��Ʈ��ũ �ھ�ܿ��� �ص��ǰ� �����ܿ��� �ص���..
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
			//	��¦ ���� �־� ���̴µ�?
			//	�ƴ� sendEvent sendBuffer�� session �����ε�
			//	�� �����忡�� WSASend ���� �ߴٰ� �������� ����ϴ� Buffer�� �� ������� Ŭ����?
			//	�ٸ� �����忡�� �׾ƿ� sendBuffer �����ʹ� �������� ���󰡴µ� �׷���?
			//	�̹� ���縦 �س��Ƽ� ����� ���� ���� �ְڴµ�?
			//	TODO : ���ݿ� ������ ���ܼ� ������ ���� ���� �±��ϴ�.
			//	�ٵ� ������ ������ �ʰ� �ٸ� ������ ���ؼ� ����� ������ ��... �ָ��ѵ�
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
	//	������ ���̱��Ѵ�..
	//	���࿡ RegisterSend�� ù��° WSASend�� �ϰ� �ι�° _sendEvent�� sendBuffer�� ��� ��
	//  ProcessSend�� ����ϴ� ��Ŀ�����尡 �ϰ��� �ʹ� �з��� ù��° WSASend�� ó������ ����
	//	_sendEvent�� ��������ϴٰ� _sendEvent�� sendBuffer�� ����ϴ� ������� ���� Ÿ�ֿ̹�
	//	ProcessSend�� ����ϴ� ������� _sendEvent�� Clear �ϰ� RegisterSend ����ϴ� �����忡�� _sendEvent�� sendBuffer ����ϸ�
	//	ũ���ó���..
	//	One Thread
	//	session Ref --

	{
		_sendEvent.SetOwner(nullptr);
		//	sendBuffers Ref --
		_sendEvent.GetSendBuffers().clear();
	}

	//	BoundedQueue .. Clear? �ָ��ѵ��̰�...
	if (numOfBytes == 0)
	{
		Disconnect(L"Send 0");
		return;
	}

	//	Contents --
	OnSend(numOfBytes);

	//	ũ����..?
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
