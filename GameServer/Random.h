#pragma once
#include <random>

class Random
{
public:
	static void Initialize();

public:
	static float GetFloatBetweenZeroAndOne();
	static int32 GetIntBetweenZeroAndOne();
	static float GetFloatRange(float min, float max);
	static int32 GetIntRange(int32 min, int32 max);

public:
	static Vector2		GetVector2(const Vector2& min, const Vector2& max);
	static Vector2Int	GetVector2Int(const Vector2Int& min, const Vector2Int& max);

private:
	static std::mt19937	s_Generator;
};

