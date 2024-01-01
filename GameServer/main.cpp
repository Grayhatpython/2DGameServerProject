#include "pch.h"
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include "Random.h"
#include "PerformanceCounter.h"
#include "GameServer.h"
#include "SocketUtils.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(3405);
	std::wcout.imbue(std::locale("korean"));
	
	Random::Initialize();
	PerformanceCounter::Initialize();

	GGlobal = new Global();
	GGlobal->Initialize();

	{	
		GameServer server;
		ASSERT(server.Initialize());
		server.Start();
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
