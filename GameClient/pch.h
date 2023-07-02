#pragma once

#define WIN32_LEAN_AND_MEAN // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif


#include "CorePch.h"
#include "Enum.pb.h"
#include "Struct.pb.h"

#include <SDL.h>

#include "Math.h"
#include "Random.h"

enum class ActorState
{
	Active,
	DeActive,
	Dead,
};

//	TEMP	
#define MOVE_DISTANCE				32.f
#define DIAGONAL_MOVE_DISTANCE		45.f
//	TEMP
class Performance
{
public:
	Performance()
	{
		QueryPerformanceFrequency(&timer);
		QueryPerformanceCounter(&start);
	}
	~Performance()
	{
		QueryPerformanceCounter(&end);

		DeltaTime = (end.QuadPart - start.QuadPart) / (float)timer.QuadPart;
		auto t = std::to_wstring(DeltaTime) + L"\n";
		::OutputDebugString(t.c_str());
	}

private:
	LARGE_INTEGER timer{}, start{}, end{};
	float DeltaTime = 0.f;
};

#define WHITE SDL_Color{255,255,255,255}