#include "pch.h"
#include "NetworkAddress.h"

NetworkAddress::NetworkAddress(SOCKADDR_IN address)
	: _address(address)
{

}

NetworkAddress::NetworkAddress(const std::wstring& ip, uint16 port)
{
	::memset(&_address, 0, sizeof(_address));
	_address.sin_family = AF_INET;
	_address.sin_addr = IpToAddress(ip.c_str());
	_address.sin_port = ::htons(port);
}

std::wstring NetworkAddress::GetIp()
{
	WCHAR ip[100];
	::InetNtopW(AF_INET, &_address.sin_addr, ip, 100);
	return std::wstring(ip);
}

IN_ADDR NetworkAddress::IpToAddress(const WCHAR* ip)
{
	IN_ADDR address;
	::InetPtonW(AF_INET, ip, &address);
	return address;
}
