#pragma once

struct Position
{
	bool operator==(const Position& other)const
	{
		return y == other.y && x == other.x;
	}

	bool operator!=(Position& other)
	{
		return !(*this == other);
	}

	bool operator<(const Position& other) const
	{
		if (y != other.y)
			return y < other.y;
		return x < other.x;
	}

	Position operator+(Position& other)
	{
		Position position;
		position.x = x + other.x;
		position.y = y + other.y;
		return position;
	}

	Position& operator+=(Position& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	int32 x = 0;
	int32 y = 0;
};

struct PositionHash 
{
	size_t operator()(const Position& position) const {
		std::size_t h1 = std::hash<int32>{}(position.x);
		std::size_t h2 = std::hash<int32>{}(position.y);
		return h1 ^ (h2 << 1);
	}
};

struct Tile
{
	Position			position{};
	bool				isBlocked = false;
};

struct TileNode
{
	bool operator<(const TileNode& other) const { return f < other.f; }
	bool operator>(const TileNode& other) const { return f > other.f; }

	int32			f = 0;
	int32			g = 0;
	Position		position{};
};

class Actor;
class Field
{
public:
	Field();
	~Field();

public:
	void	LoadData(const std::wstring& fileName);

public:
	std::vector<Vector2> PathFinding(const Vector2& startPosition, const Vector2& destPosition, bool ignoreCollisionActor = false);

public:
	bool		CanMovePosition(const Vector2& movePosition, bool ignoreActor = false);
	bool		RegisterActorsMappingTable(std::shared_ptr<Actor> actor);
	bool		ModifyActorsMappingTableByMove(std::shared_ptr<Actor> actor, const Vector2& movePosition);
	bool		DeRegisterActorsMappingTable(std::shared_ptr<Actor> actor, bool onlyDeRegisterActorMappingTable = true);

public:
	std::shared_ptr<Actor>		FindActorFromPosition(const Vector2& position);
	Vector2Int					FindFieldIndexFromPosition(const Vector2& position);

private:
	Tile*		FindTileFromPosition(const Vector2& position);

public:
	int32		GetHowManyTilesAwayFromHereToThere(const Vector2& here, const Vector2& there);
	
public:
	int32		GetRows() { return _rows; }
	int32		GetColumns() { return _columns; }

	int32		GetTileWidth() { return _tileWidth; }
	int32		GetTileHeight() { return _tileHeight; }

private:
	int32												_rows = 0;
	int32												_columns = 0;
	int32												_tileWidth = 0;
	int32												_tileHeight = 0;

	std::vector<std::vector<Tile*>>						_tiles;
	//	There is a actors mapped to that tiles location
	std::vector<std::vector<std::shared_ptr<Actor>>>	_actorsMappingTable;
};
