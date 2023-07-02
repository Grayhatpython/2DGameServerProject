#include "pch.h"
#include "Grid.h"
#include "Tile.h"
#include "ActorManager.h"

Grid::Grid()
{

}

Grid::~Grid()
{
}

void Grid::Initialize()
{
	_tiles.resize(_rows);
	for (size_t i = 0; i < _tiles.size(); i++)
		_tiles[i].resize(_columns);

	for (int32 row = 0; row < _rows; row++)
	{
		for (int32 column = 0; column < _columns; column++)
		{
			//	TODO
			auto tile = std::make_shared<Tile>();
			_tiles[row][column] = tile;
		}
	}

	for (size_t i = 0; i < _rows; i++)
	{
		for (size_t j = 0; j < _columns; j++)
		{
			if (i > 0)
			{
				_tiles[i][j]->_adjacent.push_back(_tiles[i - 1][j].get());
			}
			if (i < _rows - 1)
			{
				_tiles[i][j]->_adjacent.push_back(_tiles[i + 1][j].get());
			}
			if (j > 0)
			{
				_tiles[i][j]->_adjacent.push_back(_tiles[i][j - 1].get());
			}
			if (j < _columns - 1)
			{
				_tiles[i][j]->_adjacent.push_back(_tiles[i][j + 1].get());
			}

			if (i > 0 && j > 0)
			{
				_tiles[i][j]->_adjacent.push_back(_tiles[i - 1][j - 1].get());
			}
			if (i < _rows - 1 && j > 0)
			{
				_tiles[i][j]->_adjacent.push_back(_tiles[i + 1][j - 1].get());
			}
			if (i > 0 && j < _columns - 1)
			{
				_tiles[i][j]->_adjacent.push_back(_tiles[i - 1][j + 1].get());
			}
			if (i < _rows - 1 && j < _columns - 1)
			{
				_tiles[i][j]->_adjacent.push_back(_tiles[i + 1][j + 1].get());
			}
		}
	}
}

void Grid::SpecificActorUpdate(float deltaTime)
{
	Actor::SpecificActorUpdate(deltaTime);
}

