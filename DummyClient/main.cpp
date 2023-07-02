#include "pch.h"
#include "DummyClient.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

int main()
{
	std::this_thread::sleep_for(500ms);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	std::wcout.imbue(std::locale("korean"));

	Random::Initialize();

	GGlobal = new Global();
	GGlobal->Initialize();


	//_CrtSetBreakAlloc(3811);
	{
		std::unique_ptr<DummyClient> dummyClients = std::make_unique<DummyClient>();
		bool success = dummyClients->Initialize();
		if (success)
			dummyClients->Start();
	}

	if (GGlobal)
	{
		google::protobuf::ShutdownProtobufLibrary();
		delete GGlobal;
		GGlobal = nullptr;
	}

}
