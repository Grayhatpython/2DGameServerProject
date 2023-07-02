#include "pch.h"
#include "MapManager.h"
#include <sstream>
#include "XmlParser.h"
#include "Grid.h"
#include "Tile.h"
#include "AssetManager.h"
#include "ActorManager.h"
#include "TransformComponent.h"
#include "TileSpriteComponent.h"
#include "RenderManager.h"

MapManager* GMapManager = nullptr;

MapManager::MapManager()
{

}

MapManager::~MapManager()
{

}

void MapManager::LoadMap(const std::wstring& fileName)
{
	XmlParser parser;
	XmlNode root;

	ASSERT(parser.ParseFromFile(fileName.c_str(), root));
	int32 mapRow = root.GetInt32Attr(L"height");
	int32 mapColumn = root.GetInt32Attr(L"width");
	int32 tileWidth = root.GetInt32Attr(L"tilewidth");
	int32 tileHeight = root.GetInt32Attr(L"tileheight");

	XmlNode tileSetTable = root.FindChild(L"tileset");
	int32 tileColumns = tileSetTable.GetInt32Attr(L"columns");

	float tileScale = 2.f;

	auto texture = GAssetManager->GetTexture("../Assets/TileMap/beach.png");
	ASSERT(texture);

	_min.x = 0;
	_min.y = 0;
	_max.x = static_cast<int32>(mapColumn * tileWidth * tileScale);
	_max.y = static_cast<int32>(mapRow * tileHeight * tileScale);


	//	TEMP
	auto grid = GActorManager->AddGrid();
	grid->SetRows(mapRow);
	grid->SetColumns(mapColumn);
	grid->SetTileWidth(tileWidth);
	grid->SetTileHeight(tileHeight);
	grid->SetTileSetColumns(tileColumns);
	grid->SetTileScale(tileScale);
	grid->Initialize();

	Vector<XmlNode> layerTables = root.FindChildren(L"layer");

	std::wstring name;
	std::wstring data;
	std::wstring indexStr;

	int32 row = 0;
	int32 column = 0;

	int32 offsetX = 0;
	int32 offsetY = 0;

	Vector2Int gridSize{};

	std::vector<std::vector<TileSpriteComponent*>> tileSprites(mapRow, std::vector<TileSpriteComponent*>(mapColumn));

	auto& tiles = grid->GetTiles();

	for (auto& layerTable : layerTables)
	{
		name = layerTable.GetStringAttr(L"name");
		if (name == L"BaseLayer")
		{
			XmlNode baseLayer = layerTable.FindChild(L"data");
			data = baseLayer.GetStringValue();

			std::wstringstream stream(data);

			while (std::getline(stream, indexStr, L','))
			{
				int32 index = ::_wtoi(indexStr.c_str()) - 1;

				offsetX = index % tileColumns;
				offsetY = index / tileColumns;

				auto transform = tiles[row][column]->AddComponent<TransformComponent>();
				tileSprites[row][column] = tiles[row][column]->AddComponent<TileSpriteComponent>();
				tileSprites[row][column]->SetWidth(tileWidth);
				tileSprites[row][column]->SetHeight(tileHeight);
				tileSprites[row][column]->AddTileSpriteInfo(offsetX, offsetY);
				tileSprites[row][column]->SetOwner(tiles[row][column]);
				tileSprites[row][column]->SetTexture(texture);

				transform->SetScale(tileScale);
				Vector2 position;
				position.x = static_cast<float>((column * tileWidth * transform->GetScale()) + tileWidth);
				position.y = static_cast<float>((row * tileHeight * transform->GetScale()) + tileHeight);

				transform->SetPosition(position);

				column++;
				if (column == mapColumn)
				{
					column = 0;
					row++;
				}
			}
		}

		else if (name == L"EnvironmentLayer")
		{
			XmlNode environmentLayer = layerTable.FindChild(L"data");
			data = environmentLayer.GetStringValue();

			std::wstringstream stream(data);

			while (std::getline(stream, indexStr, L','))
			{
				int32 index = ::_wtoi(indexStr.c_str()) - 1;

				if (index > 0)
				{
					offsetX = index % tileColumns;
					offsetY = index / tileColumns;

					tileSprites[row][column]->AddTileSpriteInfo(offsetX, offsetY);
				}

				column++;
				if (column == mapColumn)
				{
					column = 0;
					row++;
				}
			}
		}
		else if(name == L"CollisionLayer")
		{
			XmlNode collisionLayer = layerTable.FindChild(L"data");
			data = collisionLayer.GetStringValue();

			std::wstringstream stream(data);

			while (std::getline(stream, indexStr, L','))
			{
				int32 isBlocked = ::_wtoi(indexStr.c_str());

				if (isBlocked)
					tiles[row][column]->SetIsBlocked(true);
				else
					tiles[row][column]->SetIsBlocked(false);

				column++;
				if (column == mapColumn)
				{
					column = 0;
					row++;
				}
			}
		}

		row = 0;
		column = 0;
	}
}

std::vector<Vector2> MapManager::PathFinding(const Vector2& startPosition, const Vector2& destPosition, bool ignoreDestPosition)
{
	auto grid = GActorManager->GetGrid();
	auto& tiles = grid->GetTiles();

	Tile* startTile = tiles[static_cast<int32>(startPosition.y / (grid->GetTileWidth() * grid->GetTileScale()))][static_cast<int32>(startPosition.x / (grid->GetTileWidth() * grid->GetTileScale()))].get();
	Tile* destTile = tiles[static_cast<int32>(destPosition.y / (grid->GetTileWidth() * grid->GetTileScale()))][static_cast<int32>(destPosition.x / (grid->GetTileWidth() * grid->GetTileScale()))].get();

	for (int32 row = 0; row < grid->_rows; row++)
	{
		for (int32 column = 0; column < grid->_columns; column++)
		{
			tiles[row][column]->_isVisited = false;
			tiles[row][column]->_f = Math::Infinity;
			tiles[row][column]->_g = Math::Infinity;
			tiles[row][column]->_parent = nullptr;
		}
	}

	auto distance = [](Tile* tile1, Tile* tile2)
	{
		auto position1 = tile1->GetComponent<TransformComponent>()->GetPosition();
		auto position2 = tile2->GetComponent<TransformComponent>()->GetPosition();
		auto diff = position1 - position2;
		return diff.Length();
	};

	auto heuristic = [distance](Tile* tile1, Tile* tile2)
	{
		return distance(tile1, tile2);
	};

	Tile* currentTile = nullptr;
	currentTile = startTile;
	currentTile->_g = 0.f;
	currentTile->_f = heuristic(currentTile, destTile);

	std::list<Tile*> tileList;
	tileList.push_back(currentTile);

	//	후보 리스트에 목록이 없거나 현재 타일의 위치가 목적이 타일이면.. 그만
	while (tileList.empty() == false && currentTile != destTile)
	{
		//	가장 비용이 저렴한 후보를 추출하기 위해 정렬...
		tileList.sort([](const Tile* tile1, const Tile* tile2)
			{
				return tile1->GetCost() < tile2->GetCost();
			});

		//	후보 리스트에 목록이 있고 이미 가장 비용이 저렴한 후보를 방문한 경우 그 후보를 퇴출
		while (tileList.empty() == false && tileList.front()->_isVisited)
			tileList.pop_front();

		//	후보 퇴출 후에 후보 리스트가 비어 있으면.. 그만
		if (tileList.empty())
			break;

		//	현재 타일을 가장 저렴한 후보로 셋팅 및 방문 
		currentTile = tileList.front();
		currentTile->_isVisited = true;

		//	현재 타일에서 인근의 이웃 타일을 서치 
		for (auto neighbour : currentTile->_adjacent)
		{
			//	방문하지 않았고 장애물이 아니면 후보 리스트에 등록
			if (neighbour->_isVisited == false && neighbour->GetIsBlocked() == false)
				tileList.push_back(neighbour);

			//	시작타일부터 이웃타일까지의 코스트 계산
			float g = currentTile->_g + distance(currentTile, neighbour);

			//	더 좋은 코스트로 이웃 타일을 찾은 경우
			if (g < neighbour->_g)
			{
				neighbour->_parent = currentTile;
				neighbour->_g = g;

				neighbour->_f = neighbour->_g + heuristic(neighbour, destTile);
			}
		}
	}

	std::vector<Tile*> parents;
	std::vector<Vector2> path;
	Tile* parent = destTile;
	parents.push_back(parent);
	while (parent->_parent != nullptr)
	{
		parent = parent->_parent;
		parents.push_back(parent);
	}

	std::reverse(parents.begin(), parents.end());

	//if (parents.size() == 0 || parents[0] != startTile)
	//	return std::vector<Vector2>();

	for (auto parent : parents)
	{
		auto position = parent->GetComponent<TransformComponent>()->GetPosition();
		path.push_back(position);
	}

	return path;
}

bool MapManager::CanMovePosition(Vector2Int movePosition)
{
	if (movePosition.x < _min.x || movePosition.x > _max.x)
		return false;
	if (movePosition.y < _min.y || movePosition.y > _max.y)
		return false;

	auto grid = GActorManager->GetGrid();
	auto& tiles = grid->GetTiles();

	int32 row = movePosition.y / (grid->GetTileHeight() * static_cast<int32>(grid->GetTileScale()));
	int32 column = movePosition.x / (grid->GetTileWidth() * static_cast<int32>(grid->GetTileScale()));

	return tiles[row][column]->GetIsBlocked() == false;
}

bool MapManager::CanMovePosition(Vector2 movePosition)
{
	Vector2Int position = Vector2ToVector2Int(movePosition);

	if (position.x < _min.x || position.x > _max.x)
		return false;
	if (position.y < _min.y || position.y > _max.y)
		return false;

	auto grid = GActorManager->GetGrid();
	auto& tiles = grid->GetTiles();

	int32 row = position.y / (grid->GetTileHeight() * static_cast<int32>(grid->GetTileScale()));
	int32 column = position.x / (grid->GetTileWidth() * static_cast<int32>(grid->GetTileScale()));

	return tiles[row][column]->GetIsBlocked() == false;
}

bool MapManager::CanMovePosition(int32 row, int32 column)
{
	auto grid = GActorManager->GetGrid();
	auto& tiles = grid->GetTiles();

	if (row < 0 || row >= grid->GetRows() || column < 0 || grid->GetColumns() >= column)
		return false;

	return tiles[row][column]->GetIsBlocked() == false;
}



