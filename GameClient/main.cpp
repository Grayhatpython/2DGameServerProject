#include "pch.h"
#include "Game.h"
#include "ThreadManager.h"

#undef main

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

int main()
{
	std::this_thread::sleep_for(3000ms);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	std::wcout.imbue(std::locale("korean"));

	Random::Initialize();

	GGlobal = new Global();
	GGlobal->Initialize();


	//_CrtSetBreakAlloc(4422);
	{
		std::unique_ptr<Game> game = std::make_unique<Game>();
		bool success = game->Initialize();
		if (success)
			game->Run();
	}

	if (GGlobal)
	{
		google::protobuf::ShutdownProtobufLibrary();
		delete GGlobal;
		GGlobal = nullptr;
	}

	return 0;
}