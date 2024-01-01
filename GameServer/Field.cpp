#include "pch.h"
#include "Field.h"
#include "pch.h"
#include <sstream>
#include "XmlParser.h"

#include "Region.h"
#include "Actor.h"
#include "Player.h"
#include "Monster.h"
#include "Projectile.h"

Field::Field()
{

}

Field::~Field()
{
	std::wcout << "~Field" << std::endl;

	for (auto row : _tiles)
	{
		for (auto column : row)
		{
			delete column;
			column = nullptr;
		}
	}

	_tiles.clear();
}

void Field::LoadData(const std::wstring& fileName)
{
	XmlParser parser;
	XmlNode root;

	ASSERT(parser.ParseFromFile(fileName.c_str(), root));

	_rows = root.GetInt32Attr(L"height");
	_columns = root.GetInt32Attr(L"width");
	_tileWidth = static_cast<int32>(root.GetInt32Attr(L"tilewidth") * TILE_SCALE);
	_tileHeight = static_cast<int32>(root.GetInt32Attr(L"tileheight") * TILE_SCALE);

	_tiles.resize(_rows, std::vector<Tile*>(_columns, nullptr));
	_actorsMappingTable.resize(_rows, std::vector<std::shared_ptr<Actor>>(_columns, nullptr));

	for (int32 row = 0; row < _rows; row++)
	{
		for (int32 column = 0; column < _columns; column++)
		{
			auto tile = new Tile();
			_tiles[row][column] = tile;
		}
	}

	Vector<XmlNode> layerTables = root.FindChildren(L"layer");

	std::wstring name;
	std::wstring data;
	std::wstring indexStr;

	int32 row = 0;
	int32 column = 0;

	int32 offsetX = 0;
	int32 offsetY = 0;

	Vector2 position;

	for (auto& layerTable : layerTables)
	{
		name = layerTable.GetStringAttr(L"name");

		if (name == L"CollisionLayer")
		{
			XmlNode collisionLayer = layerTable.FindChild(L"data");
			data = collisionLayer.GetStringValue();

			std::wstringstream stream(data);

			while (std::getline(stream, indexStr, L','))
			{
				int32 isBlocked = ::_wtoi(indexStr.c_str());

				_tiles[row][column]->position.x = column * _tileWidth + (_tileWidth / 2);
				_tiles[row][column]->position.y = row * _tileHeight + (_tileHeight / 2);

				if (isBlocked)
					_tiles[row][column]->isBlocked = true;

				column++;
				if (column == _columns)
				{
					column = 0;
					row++;
				}
			}
		}
	}
}

//	ignoreCollisionActor (false)	->	Actor�鰣�� �浹�� �����ؼ� ���� ã�� ���̴�
//	ignoreCollisionActor (true)		->	Actor���� �浹�� ������ ä ������ ���� Ÿ�������� ���ؼ��� ���� ã�� ���̴�
std::vector<Vector2> Field::PathFinding(const Vector2& startPosition, const Vector2& destPosition, bool ignoreCollisionActor)
{
	const int32 neightborhoodDirectionCount = 8;
	const int32	pathFindingMaxDistance = 10;

	Tile* start = FindTileFromPosition(startPosition);
	Tile* dest = FindTileFromPosition(destPosition);

	Position neighborhoodPosition[] =
	{
		Position { 0, -TILE_DISTANCE},				//	UP
		Position { -TILE_DISTANCE, 0},				//	LEFT
		Position { 0, TILE_DISTANCE},				//	DOWN
		Position { TILE_DISTANCE, 0},				//	RIGHT
		/*
		Position { -TILE_DISTANCE, -TILE_DISTANCE},	//	UP_LEFT
		Position {-TILE_DISTANCE, TILE_DISTANCE},	//	DOWN_LEFT
		Position {TILE_DISTANCE, TILE_DISTANCE},	//	DOWN_RIGHT
		Position { TILE_DISTANCE, -TILE_DISTANCE}	//	UP_RIGHT
		*/
	};

	int32 cost[] =
	{
		STRAIGHT_COST,	//	UP
		STRAIGHT_COST,	//	LEFT
		STRAIGHT_COST,	//	DOWN
		STRAIGHT_COST,	//	RIGHT
		/*
		DIAGONAL_COST,	//	UP_LEFT
		DIAGONAL_COST,	//	DOWN_LEFT
		DIAGONAL_COST,	//	DOWN_RIGHT
		DIAGONAL_COST	//	UP_RIGHT
		*/
	};

	HashMap<Position, bool, PositionHash>		closedList;
	HashMap<Position, int32, PositionHash>		bestCostList;
	Map<Position, Position>						parentList;
	PriorityQueue<TileNode, Vector<TileNode>, greater<TileNode>> openList;

	{
		int32 g = 0;
		int32 h = STRAIGHT_COST * (abs(dest->position.y - start->position.y) + abs(dest->position.x - start->position.x));
		int32 f = g + h;
		openList.push(TileNode{f, g, start->position});
		bestCostList[start->position] = f;
		parentList[start->position] = start->position;
	}

	while (openList.empty() == false)
	{
		auto currentTileNode = openList.top();
		openList.pop();

		if (closedList.find(currentTileNode.position) != closedList.end())
			continue;
		auto best = bestCostList.find(currentTileNode.position);
		if (best != bestCostList.end())
		{
			if (best->second < currentTileNode.f)
				continue;
		}

		closedList[currentTileNode.position] = true;

		if (currentTileNode.position == dest->position)
			break;

		for (int32 dir = 0; dir < neightborhoodDirectionCount; dir++)
		{
			Position nextTilePosition = currentTileNode.position + neighborhoodPosition[dir];
			Vector2 tempPosition{ static_cast<float>(nextTilePosition.x),static_cast<float>(nextTilePosition.y) };

			auto distanceX = abs(start->position.x - nextTilePosition.x) / TILE_DISTANCE;
			auto distanceY = abs(start->position.y - nextTilePosition.y) / TILE_DISTANCE;

			if (distanceX + distanceY > pathFindingMaxDistance)
				continue;

			if (dest->position != nextTilePosition && CanMovePosition(tempPosition) == false)
				continue;

			if (closedList.find(nextTilePosition) != closedList.end())
				continue;

			int32 g = currentTileNode.g + cost[dir];
			int32 h = STRAIGHT_COST * (abs(dest->position.y - nextTilePosition.y) + abs(dest->position.x - nextTilePosition.x));
			int32 f = g + h;

			auto best = bestCostList.find(nextTilePosition);
			if (best != bestCostList.end())
			{
				if (best->second < f)
					continue;
			}

			//	����
			bestCostList[nextTilePosition] = f;
			parentList[nextTilePosition] = currentTileNode.position;
			openList.push(TileNode{ f, g, nextTilePosition });
		}
	}

	std::vector<Vector2> path;

	auto finalDestPosition = dest->position;

	//	���� �� ã�� ��� -> ���� �輱...
	if (parentList.find(dest->position) == parentList.end())
	{
		Position bestPosition{};
		int32 bestDistance = INT_MAX;

		for (auto& position : parentList)
		{
			auto distanceX = abs(dest->position.x - position.first.x) / TILE_DISTANCE;
			auto distanceY = abs(dest->position.y - position.first.y) / TILE_DISTANCE;
			int32 currentDistance = distanceX + distanceY;

			if (currentDistance < bestDistance)
			{
				bestPosition = position.first;
				bestDistance = currentDistance;
			}
		}

		finalDestPosition = bestPosition;
	}

	while (true)
	{
		Vector2 tempPosition{ static_cast<float>(finalDestPosition.x),static_cast<float>(finalDestPosition.y) };
		path.push_back(tempPosition);

		if (finalDestPosition == parentList[finalDestPosition])
			break;

		finalDestPosition = parentList[finalDestPosition];
	}

	std::reverse(path.begin(), path.end());

	return path;
}

bool Field::CanMovePosition(const Vector2& movePosition, bool ignoreActor)
{
	auto moveTile = FindTileFromPosition(movePosition);

	return moveTile && moveTile->isBlocked == false && (ignoreActor || FindActorFromPosition(movePosition) == nullptr);
}

bool Field::RegisterActorsMappingTable(std::shared_ptr<Actor> actor)
{
	auto position = actor->GetPosition();
	auto region = actor->GetRegion();
	ASSERT(region);

	auto zone = region->FindZoneFromPosition(position);
	ASSERT(zone);

	zone->AddActor(actor);

	if (actor->GetType() == Protocol::ActorType::PROJECTILE)
		return true;

	auto registerFieldIndex = FindFieldIndexFromPosition(position);
	ASSERT(registerFieldIndex.x >= 0 || registerFieldIndex.y >= 0);

	auto registerActor = _actorsMappingTable[registerFieldIndex.y][registerFieldIndex.x];
	
	if (registerActor == nullptr)
		_actorsMappingTable[registerFieldIndex.y][registerFieldIndex.x] = actor;
	//	����ҷ��µ� �������� �ֳ�?? ����??
	else
		ASSERT(false);

	return true;
}

bool Field::ModifyActorsMappingTableByMove(std::shared_ptr<Actor> actor, const Vector2& movePosition)
{
	if (CanMovePosition(movePosition) == false)
		return false;
	
	auto position = actor->GetPosition();
	auto region = actor->GetRegion();
	ASSERT(region);

	//	������ ��ġ�� �ش��ϴ� ���� �������� �̵��� ��ġ�� �ش��ϴ� ���� ã�ƿ´�..
	auto currentZone = region->FindZoneFromPosition(position);
	ASSERT(currentZone);
	auto nextMoveZone = region->FindZoneFromPosition(movePosition);
	ASSERT(nextMoveZone);

	if (actor->GetType() == Protocol::ActorType::PROJECTILE)
	{
		//	�̵����� ���ؼ� ���� ����Ǵ� ��� -> ���� ������ ���� �����ϰ� �����̴� ���� �ش��ϴ� ������ ����Ѵ�..
		if (currentZone != nextMoveZone)
		{
			currentZone->RemoveActor(actor);
			nextMoveZone->AddActor(actor);
		}
	}
	else
	{
		auto moveFieldIndex = FindFieldIndexFromPosition(movePosition);
		ASSERT(moveFieldIndex.x >= 0 || moveFieldIndex.y >= 0);

		//	���� ��ġ�� �̵��ҷ��� �÷��̾� ���̺��� ���� ���� ��ġ���� ������� ���캸�µ�
		//	�÷��̾� ���̺� ��ϵǾ� �ִ� ������ �� �ڽ��� �ƴϰ� �ٸ� Actor�� �ְų� 
		//	������ Crash ����... ������ �ִ� ����
		//	������ ����ߴµ� ���� ���� ���ų� �ٸ� ����̹Ƿ�

		DeRegisterActorsMappingTable(actor);

		_actorsMappingTable[moveFieldIndex.y][moveFieldIndex.x] = actor;

		//	�̵����� ���� Zone ���� .. ������ �ӽ� �ڵ�

		//	������ ��ġ�� �ش��ϴ� ���� �������� �̵��� ��ġ�� �ش��ϴ� ���� ã�ƿ´�..
		auto currentZone = region->FindZoneFromPosition(position);
		ASSERT(currentZone);
		auto nextMoveZone = region->FindZoneFromPosition(movePosition);
		ASSERT(nextMoveZone);

		//	�̵����� ���ؼ� ���� ����Ǵ� ��� -> ���� ������ ���� �����ϰ� �����̴� ���� �ش��ϴ� ������ ����Ѵ�..
		if (currentZone != nextMoveZone)
		{
			currentZone->RemoveActor(actor);
			nextMoveZone->AddActor(actor);
		}
	}

	//	���...
	actor->SetPosition(movePosition);

	return true;
}

bool Field::DeRegisterActorsMappingTable(std::shared_ptr<Actor> actor, bool onlyDeRegisterActorMappingTable)
{
	auto position = actor->GetPosition();

	auto region = actor->GetRegion();
	ASSERT(region);

	if (onlyDeRegisterActorMappingTable == false)
	{
		auto zone = region->FindZoneFromPosition(position);
		ASSERT(zone);

		zone->RemoveActor(actor);

		if (actor->GetType() == Protocol::ActorType::PROJECTILE)
			return true;
	}

	auto leaveFieldIndex = FindFieldIndexFromPosition(position);
	ASSERT(leaveFieldIndex.x >= 0 || leaveFieldIndex.y >= 0);

	auto registedActor = _actorsMappingTable[leaveFieldIndex.y][leaveFieldIndex.x];

	//	���� ��ġ�� ���� �°� �� �ִ� ��� -> �ùٸ� ���
	if (registedActor == actor)
	{
		//	���� ���� �ִ� ��ġ���� ������ �����.
		_actorsMappingTable[leaveFieldIndex.y][leaveFieldIndex.x] = nullptr;
	}
	else
		//	���� ���� �ִ� ��ġ�� �ٸ� �ְ� �ִ�..? ����?
		ASSERT(false);

	return true;

}

std::shared_ptr<Actor> Field::FindActorFromPosition(const Vector2& position)
{
	auto fieldIndex = FindFieldIndexFromPosition(position);
	if (fieldIndex.x < 0 || fieldIndex.y < 0)
		return nullptr;

	return _actorsMappingTable[fieldIndex.y][fieldIndex.x];;
}

Tile* Field::FindTileFromPosition(const Vector2& position)
{
	auto fieldIndex = FindFieldIndexFromPosition(position);
	if (fieldIndex.x < 0 || fieldIndex.y < 0)
		return nullptr;

	return _tiles[fieldIndex.y][fieldIndex.x];
}

Vector2Int Field::FindFieldIndexFromPosition(const Vector2& position)
{
	if(position.x < 0 || position.y < 0)
		return Vector2Int{ -1, -1 };

	int32 row = static_cast<int32>(position.y) / _tileHeight;
	int32 column = static_cast<int32>(position.x) / _tileWidth;

	//	������ ��� ��ġ??? ��Ȳ ����..
	if(row < _rows && row >= 0 && column < _columns && column >= 0)
		return Vector2Int{ column, row };
	
	return Vector2Int{ -1, -1};
}

int32 Field::GetHowManyTilesAwayFromHereToThere(const Vector2& here, const Vector2& there)
{
	auto directionVector = here - there;
	auto distance = directionVector.Length();
	auto awayTilesCount = distance / MOVE_DISTANCE;
	return static_cast<int32>(awayTilesCount);

}
