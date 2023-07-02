#pragma once
#include "Protocol.pb.h"

#include "Zone.h"
#include "Field.h"

class Actor;
class Player;
class Monster;
class Projectile;
class Region : public JobBaseObject
{
public:
	Region();
	~Region();

public:
	bool Initialize(int32 splitZoneCount);

public:
	void Enter(std::shared_ptr<Actor> newActor,  bool randomPositionSpawn = true);
	void Leave(int32 actorId);

	void Move(std::shared_ptr<Player> player, Protocol::C_MOVE movePacket);
	void Skill(std::shared_ptr<Player> player, Protocol::C_SKILL skillPacket);

	void ChangeState(std::shared_ptr<Player> player, Protocol::C_CHANGE_STATE changeStatePacket);
	void ChangeMoveDir(std::shared_ptr<Player> player, Protocol::C_CHANGE_MOVE_DIR changeMoveDirPacket);

	void ChatMessage(std::shared_ptr<Player> player, Protocol::C_CHAT chatPacket);

	void EquipItem(std::shared_ptr<Player> player, Protocol::C_EQUIP_ITEM equipItemPacket);
	void BroadCast(const Vector2& position, std::shared_ptr<SendBuffer> sendBuffer);

public:
	void Update();
	void Clear();

public:
	Zone*											FindZoneFromPosition(const Vector2& position, bool findAdjacentZone = false);
	Set<Zone*>										FindAdjacentZones(const Vector2& position, int32 detectionRange = 15);

	std::shared_ptr<Player>							FindNearByPlayer(const Vector2& position, int32 searchRange);
	Vector<std::shared_ptr<Player>>					FindPlayersFromAdjacentZones(const Vector2& position, int32 searchRange);

public:
	const HashMap<int32, std::shared_ptr<Player>>&	GetPlayers() const { return _players; }

	void											SetRegionId(int32 regionId) { _regionId = regionId; }
	Field*											GetField() { return _field.get(); }

	int32											GetSplitZoneCount() { return _splitZoneCount; }
	//	TODO	: 탐지 범위?
	int32											GetDectionRange() { return 15; }

private:
	int32											_regionId = 0;
	//	Ref 사이클 체크!
	HashMap<int32, std::shared_ptr<Player>>			_players;
	HashMap<int32, std::shared_ptr<Monster>>		_monsters;
	HashMap<int32, std::shared_ptr<Projectile>>		_projectiles;

	Vector<int32>									_removePlayersId;
	Vector<int32>									_removeMonstersId;
	Vector<int32>									_removeProjectilesId;

	std::unique_ptr<Field>							_field;

	//	TEMP
	std::vector<std::vector<std::unique_ptr<Zone>>>	_zones;
	int32											_splitZoneCount = 0;
};			

