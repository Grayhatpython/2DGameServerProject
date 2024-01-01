#pragma once

#define WIN32_LEAN_AND_MEAN // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif

#pragma comment(lib, "winmm.lib")

#include "CorePch.h"
#include "Enum.pb.h"
#include "Struct.pb.h"
//	TEMP	
#define MOVE_DISTANCE			32.f
#define TILE_DISTANCE			32
#define STRAIGHT_COST			32
#define DIAGONAL_COST			45
#define PROJECTILE_DISTANCE		18.f
#define TILE_SCALE				2.f
#define TILE_STANDARD_OFFSET	16.f

#include "Math.h"

