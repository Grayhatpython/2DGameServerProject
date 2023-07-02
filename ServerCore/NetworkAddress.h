#pragma once

class NetworkAddress
{
public:
	NetworkAddress() = default;
	NetworkAddress(SOCKADDR_IN address);
	NetworkAddress(const std::wstring& ip, uint16 port);

public:
	SOCKADDR_IN&	GetAddress() { return _address; }
	std::wstring	GetIp();
	uint16			GetPort() { return ::ntohs(_address.sin_port); }

public:
	static IN_ADDR IpToAddress(const WCHAR* ip);

private:
	SOCKADDR_IN _address{};
};

