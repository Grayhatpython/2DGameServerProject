#pragma once
#include "Actor.h"

class Tile;
class Grid : public Actor
{
	friend class MapManager;

public:
	Grid();
	virtual ~Grid();

public:
	virtual void	Initialize() override;
	virtual void	SpecificActorUpdate(float deltaTime) override;

public:
	void SetRows(int32 row) { _rows = row; }
	void SetColumns(int32 column) { _columns = column; }
	void SetTileWidth(int32 width) { _tileWidth = width; }
	void SetTileHeight(int32 column) { _tileHeight = column; }
	void SetTileSetColumns(int32 columns) { _tileSetColumns = columns; }
	void SetTileScale(float scale) { _tileScale = scale; }

	int32 GetRows() const { return _rows; }
	int32 GetColumns() const { return _columns; }
	int32 GetTileWidth() const { return _tileWidth; }
	int32 GetTileHeight() const { return _tileHeight; }
	int32 GetTileSetColumns() const { return _tileSetColumns; }
	float GetTileScale() const { return _tileScale; }

	std::vector<std::vector<std::shared_ptr<Tile>>>&	GetTiles() { return _tiles; }

private:
	std::vector<std::vector<std::shared_ptr<Tile>>>		_tiles;

	int32		_rows = 0;
	int32		_columns = 0;
	int32		_tileWidth = 0;
	int32		_tileHeight = 0;
	float		_tileScale = 0.f;

	int32		_tileSetColumns = 0;
};

