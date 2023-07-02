#include "pch.h"
#include "LoginServer.h"
#include "SocketUtils.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <time.h>

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	GGlobal = new Global();
	GGlobal->Initialize();

	{
		std::unique_ptr<LoginServer> server = std::make_unique<LoginServer>();
		ASSERT(server->Initialize());
		server->Start();
	}

	if (GGlobal)
	{
		google::protobuf::ShutdownProtobufLibrary();
		delete GGlobal;
		GGlobal = nullptr;
	}

	SocketUtils::Clear();
	return 0;
}