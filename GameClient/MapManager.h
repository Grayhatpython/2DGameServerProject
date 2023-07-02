#pragma once

class MapManager
{
public:
	MapManager();
	~MapManager();

public:
	void LoadMap(const std::wstring& fileName);

public:
	std::vector<Vector2> PathFinding(const Vector2& startPosition, const Vector2& destPosition, bool ignoreDestPosition = false);

public:
	bool CanMovePosition(Vector2Int movePosition);
	bool CanMovePosition(Vector2 movePosition);
	bool CanMovePosition(int32 row, int32 column);

public:
	Vector2Int GetMapMin() const { return _min; }
	Vector2Int GetMapMax() const { return _max; }

private:
	const MapManager& operator = (const MapManager&) = delete;
	MapManager(const MapManager&) = delete;

private:
	Vector2Int					_min{};
	Vector2Int					_max{};
};

extern MapManager* GMapManager;
