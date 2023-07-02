#include "pch.h"
#include "Region.h"
#include "Field.h"
#include "Monster.h"
#include "Projectile.h"
#include "Ball.h"
#include "Player.h"
#include "ActorManager.h"
#include "ClientSession.h"
#include "Protocol.pb.h"
#include "ClientPacketHandler.h"

#include "DataManager.h"

#include "Item.h"
#include "Inventory.h"

#include "DBProcess.h"

#include "Random.h"

Region::Region()
{

}

Region::~Region()
{
	std::cout << "~Region" << std::endl;
}

bool Region::Initialize(int32 splitZoneCount)
{
	{
		//	TEMP
		_field = std::make_unique<Field>();
		_field->LoadData(L"../Common/GameData/Field/TileMap.tmx");
	}

	{
		//	Zone
		_splitZoneCount = splitZoneCount;
		auto rows = _field->GetRows();
		auto columns = _field->GetColumns();

		auto zoneRowsCount = (rows + splitZoneCount - 1) / splitZoneCount;
		auto zoneColumnsCount = (columns + splitZoneCount - 1) / splitZoneCount;

		_zones.resize(zoneRowsCount);

		for (int32 row = 0; row < zoneRowsCount; row++)
		{
			for (int32 column = 0; column < zoneColumnsCount; column++)
				_zones[row].push_back(std::make_unique<Zone>(row, column));
		}

	}

	//	Monster Test
	//{
	//	for (int32 i = 0; i < 300; i++)
	//	{
	//		//	TEMP
	//		auto monster = GActorManager->AddActor<Monster>();
	//		std::string name = "Monster_" + std::to_string(monster->GetId());
	//		monster->SetName(name);
	//		monster->Initialize();
	//		monster->SetPosition(Vector2{ 336.f + (i * MOVE_DISTANCE),336.f});
	//		OnlyPushJobAndNotDistribute(&Room::Enter, std::static_pointer_cast<Actor>(monster), true);
	//	}
	//}

	return true;
}

void Region::Enter(std::shared_ptr<Actor> newActor, bool randomPositionSpawn)
{
	ASSERT(newActor);
	auto actorType = ActorManager::GetActorTypeFromId(newActor->GetId());
	
	if (randomPositionSpawn)
	{
		Vector2 randomSpawnPosition{};
		auto rows = _field->GetRows();
		auto columns = _field->GetColumns();
		float randomRow = 0.f;
		float randomColumn = 0.f;

		//	맵이 꽉차면..?
		while (true)
		{
			randomRow = static_cast<float>(Random::GetIntRange(0, columns - 1));
			randomColumn = static_cast<float>(Random::GetIntRange(0, rows - 1));

			randomSpawnPosition.x = TILE_STANDARD_OFFSET + (randomColumn * MOVE_DISTANCE);
			randomSpawnPosition.y = TILE_STANDARD_OFFSET + (randomRow * MOVE_DISTANCE);

			if (_field->CanMovePosition(randomSpawnPosition))
			{
				newActor->SetPosition(randomSpawnPosition);
				break;
			}
		}
	}

	if (actorType == Protocol::ActorType::PLAYER)
	{
		auto newPlayer = std::static_pointer_cast<Player>(newActor);
		_players.insert(std::make_pair(newActor->GetId(), newPlayer));

		auto region = std::static_pointer_cast<Region>(shared_from_this());
		newPlayer->SetRegion(region);

		//	스텟 계산..
		newPlayer->ReCalculationAdditionalStat();

		//	필드에 플레이어 위치 등록...
		{
			_field->RegisterActorsMappingTable(newPlayer);
		}

		{
			//	새로운 플레이어의 클라이언트에게 룸에 접속이 완료됬으므로 본인 플레이어 정보 클라이언트에게 전송
			Protocol::S_ENTER_GAME enterGamePacket;
			auto player = enterGamePacket.mutable_player();
			player->MergeFrom(newPlayer->GetInfo());
			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterGamePacket);
			newPlayer->GetSession()->Send(sendBuffer);
		}
	}
	else if (actorType == Protocol::ActorType::MONSTER)
	{
		auto newMonster = std::static_pointer_cast<Monster>(newActor);
		_monsters.insert(std::make_pair(newActor->GetId(), newMonster));

		auto region = std::static_pointer_cast<Region>(shared_from_this());
		newMonster->SetRegion(region);

		//	필드에 몬스터 위치 등록
		{
			_field->RegisterActorsMappingTable(newMonster);
		}
	}
	else if (actorType == Protocol::ActorType::PROJECTILE)
	{
		auto newProjectile = std::static_pointer_cast<Projectile>(newActor);
		_projectiles.insert(std::make_pair(newActor->GetId(), newProjectile));

		auto region = std::static_pointer_cast<Region>(shared_from_this());
		newProjectile->SetRegion(region);

		//	업데이트 애매하다..
		//newProjectile->Update();

		{
			//	화살은 Zone만 관리
			_field->RegisterActorsMappingTable(newProjectile);
		}
	}
	else
		ASSERT(false);

	{
		//	다른 플레이어의 클라이언트에게 어떠한 종류의 Actor가 룸에 입장했다는 것을 알린다.
		//	만약 자신의 플레이어가 접속했을 경우에는 위에서 ENTER_GAME_PACKET으로 보내기 떄문에 여기서는 생략된다.
		Protocol::S_SPAWN spawnPacket;
		auto p = spawnPacket.add_actors();
		p->MergeFrom(newActor->GetInfo());

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(spawnPacket);
		BroadCast(newActor->GetPosition(), sendBuffer);
	}
}

void Region::Leave(int32 actorId)
{
	auto actorType = ActorManager::GetActorTypeFromId(actorId);
	Vector2 position{};

	if (actorType == Protocol::ActorType::PLAYER)
	{
		//	TEMP
		auto iter = _players.find(actorId);

		std::shared_ptr<Player> leavePlayer;
		if (iter != _players.end())
		{
			leavePlayer = iter->second;
			position = leavePlayer->GetPosition();

			//	필드에서 위치 등록 해제
			_field->DeRegisterActorsMappingTable(leavePlayer, false);

			//	DB 정보 저장
			leavePlayer->OnLeaveGame();

			//	Owner 같은 경우는 지금 게임이 죽으면 그 플레이어를 재활용하기 때문에 Onwer는 초기화 하지않는다..
			//	필드 시야각 정보 초기화..
			auto fieldOfView = leavePlayer->GetFieldOfView();
			ASSERT(fieldOfView);
			fieldOfView->Clear();

			//	룸 해제
			leavePlayer->SetRegion(nullptr);
			_players.erase(actorId);
			//_removePlayersId.push_back(actorId);

		}
		else
			ASSERT(false);

		{
			//	지금은 나 자신이 죽어도 다시 바로 EnterGame으로 부활해주므로 아직은 괜찮음
			//	본인한테 굳이 게임을 나갔다는 패킷을 보낼 필요가 었을까??
			Protocol::S_LEAVE_GAME leavePacket;
			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(leavePacket);

			{	
				//	Disconnected 처리
				leavePlayer->GetSession()->Send(sendBuffer);
				leavePlayer->SetSession(nullptr);
			}
		}
	}
	else if (actorType == Protocol::ActorType::MONSTER)
	{
		auto iter = _monsters.find(actorId);

		std::shared_ptr<Monster> leaveMonster;
		if (iter != _monsters.end())
		{
			leaveMonster = iter->second;
			position = leaveMonster->GetPosition();

			//	필드에서 위치 등록 해제
			_field->DeRegisterActorsMappingTable(leaveMonster, false);

			leaveMonster->SetRegion(nullptr);
			_monsters.erase(actorId);

			//_removeMonstersId.push_back(actorId);
		}
		else
			ASSERT(false);

	}
	else if (actorType == Protocol::ActorType::PROJECTILE)
	{
		auto iter = _projectiles.find(actorId);

		std::shared_ptr<Projectile> leaveProjectile;
		if (iter != _projectiles.end())
		{
			leaveProjectile = iter->second;
			position = leaveProjectile->GetPosition();

			//	화살은 Zone 관리만
			_field->DeRegisterActorsMappingTable(leaveProjectile, false);

			leaveProjectile->SetRegion(nullptr);
			_removeProjectilesId.push_back(actorId);
		}
		else
			ASSERT(false);
	}

	{
		//	다른 플레이어들에게 해당 Actor가 없어졌다는 패킷 전송
		Protocol::S_DESPAWN despawnPacket;
		despawnPacket.add_actorid(actorId);
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(despawnPacket);
		BroadCast(position, sendBuffer);
	}
}

void Region::Move(std::shared_ptr<Player> player, Protocol::C_MOVE movePacket)
{
	//	Validate
	ASSERT(player);

	{

		//	TEMP
		//player->SetUsedSkillId(Protocol::SkillType::SKILL_NONE);

		//	Move Validate
		//	서버에서 좌표 이동?
		//	지금 이게 이동 패킷이 오면 시뮬레이션 안하고 바로 좌표를 이동해버린다.
		//	TODO. 서버에서도 클라와 마찬가지로 바로 좌표이동시키는게 아니라 프레임 돌면서 이동 동기화 해줘야한다.	
		//player->SetPositionInfo(movePacket->positioninfo());
		const auto& movePacketPositionInfo = movePacket.positioninfo();
		auto movePosition = Vector2{ movePacketPositionInfo.positionx(), movePacketPositionInfo.positiony() };
		auto playerPosition = player->GetPosition();

		//	이동을 희망하는 좌표와 현재 이동을 요청한 플레이어의 위치가 같은면 
		//	상태만 변경되는 경우
		if (Math::NearZero((movePosition - playerPosition).Length()) == false)
		{
			//	필드에서 타일의 이동 가능 여부와 타일에 플레이어가 존재하는 경우 크로스 체크
			if (_field->CanMovePosition(movePosition) == false)
				return;
		}

		{
			//	이동가능 할 경우
			//	지금 서버에서는 프레임 루프가 없다
			//	그래서 이동 로직이 바로 처리된다.. 추후에 업데이트 예정
			player->SetState(movePacketPositionInfo.state());
			player->SetMoveDir(movePacketPositionInfo.movedir());
			//	필드에서 플레이어 등록
			_field->ModifyActorsMappingTableByMove(player, movePosition);
		}
	}

	{
		//	다른 유저에게 해당 유저가 이동했다 정보 전송
		//	바로 보내는게 맞을까 ? 검증하고 바로 보낼까 ?
		//	서버에서 시물레이션 후에 도착해서 보내면 반응이 좀 늦을 거 같은데..?
		Protocol::S_MOVE sendMovePacket;
		sendMovePacket.set_actorid(player->GetId());
		auto positionInfo = sendMovePacket.mutable_positioninfo();
		positionInfo->CopyFrom(movePacket.positioninfo());

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(sendMovePacket);
		BroadCast(player->GetPosition(), sendBuffer);
	}
}

void Region::Skill(std::shared_ptr<Player> player, Protocol::C_SKILL skillPacket)
{
	//	Validate
	ASSERT(player);

	{
		//	Skill Use Validate
		auto playerInfo = player->GetInfo();
		if (playerInfo.positioninfo().state() != Protocol::AIState::IDLE)
			return;

		//	Skill Logic ? 처리 ... 
		const auto& skillData = DataManager::FindSkillData(skillPacket.skillinfo().skillid());
		if (skillData.type == Protocol::SkillType::SKILL_NONE)
			return;

		//	Change State -> Idle to Skill
		player->SetState(Protocol::AIState::Skill);
		player->SetUsedSkillId(skillPacket.skillinfo().skillid());

		//	Send Other Player -> I Use Skill
		Protocol::S_SKILL sendSkillPacket;
		sendSkillPacket.set_actorid(player->GetId());
		//	TEMP
		auto skillId = skillPacket.skillinfo().skillid();
		sendSkillPacket.mutable_skillinfo()->set_skillid(skillId);
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(sendSkillPacket);
		BroadCast(player->GetPosition(), sendBuffer);

		switch (skillData.type)
		{
		case Protocol::SkillType::SKILL_AUTO_ATTACK:
			{
				//	TODO : Object Damage Judgment
				auto skillTargetPosition = player->GetForwardPosition();
				auto target = _field->FindActorFromPosition(skillTargetPosition);
				if (target)
				{
					//	TEMP
					target->OnDamaged(player, player->GetTotalAttack() + skillData.damage);
				}
			}
			break;
		case Protocol::SkillType::SKILL_PROJECTILE_ATTACK:
			{
				//	TODO
				auto ball = GActorManager->AddActor<Ball>();
				ASSERT(ball);

				ball->SetOwner(player);
				auto positionInfo = ball->GetPositionInfo();
				ball->SetMoveDir(player->GetMoveDir());
				ball->SetState(Protocol::AIState::MOVE);
				ball->SetPosition(player->GetPosition());
				ball->SetSpeed(skillData.projectileInfo.speed);
				Enter(ball, false);
			}
			break;
		}
		
	}
}

void Region::ChangeState(std::shared_ptr<Player> player, Protocol::C_CHANGE_STATE changeStatePacket)
{
	ASSERT(player);

	if (changeStatePacket.state() != Protocol::AIState::Skill)
		player->SetUsedSkillId(Protocol::SkillType::SKILL_NONE);

	player->SetState(changeStatePacket.state());

	Protocol::S_CHANGE_STATE changeStateSendPacket;
	changeStateSendPacket.set_actorid(player->GetId());
	changeStateSendPacket.set_state(changeStatePacket.state());
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(changeStateSendPacket);
	BroadCast(player->GetPosition(), sendBuffer);
}

void Region::ChangeMoveDir(std::shared_ptr<Player> player, Protocol::C_CHANGE_MOVE_DIR changeMoveDirPacket)
{
	ASSERT(player);

	player->SetMoveDir(changeMoveDirPacket.movedir());

	Protocol::S_CHANGE_MOVE_DIR changeMoveDirSendPacket;
	changeMoveDirSendPacket.set_actorid(player->GetId());
	changeMoveDirSendPacket.set_movedir(changeMoveDirPacket.movedir());
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(changeMoveDirSendPacket);
	BroadCast(player->GetPosition(), sendBuffer);
}

void Region::ChatMessage(std::shared_ptr<Player> player, Protocol::C_CHAT chatPacket)
{
	ASSERT(player);

	Protocol::S_CHAT chatSendPacket;
	chatSendPacket.set_actorid(player->GetId());
	chatSendPacket.set_chat(chatPacket.chat());
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(chatSendPacket);
	BroadCast(player->GetPosition(), sendBuffer);
}

void Region::EquipItem(std::shared_ptr<Player> player, Protocol::C_EQUIP_ITEM equipItemPacket)
{
	ASSERT(player);
	player->EquipItem(equipItemPacket);
}

//	존에 해당하는 플레이어들에게만 BroadCast를 한다..
void Region::BroadCast(const Vector2& position, std::shared_ptr<SendBuffer> sendBuffer)
{
	auto adjacentZones = FindAdjacentZones(position);

	for (auto zone : adjacentZones)
	{
		auto players = zone->GetPlayers();
		
		for (auto& player : players)
		{
			auto session = player->GetSession();
			ASSERT(session);

			auto playerTileIndex = _field->FindFieldIndexFromPosition(player->GetPosition());
			ASSERT(playerTileIndex.x >= 0 || playerTileIndex.y >= 0);
			auto positionTileIndex = _field->FindFieldIndexFromPosition(position);
			ASSERT(positionTileIndex.x >= 0 || positionTileIndex.y >= 0);

			auto diff = playerTileIndex - positionTileIndex;
			if (std::abs(diff.x) > GetDectionRange())
				continue;
			if (std::abs(diff.y) > GetDectionRange())
				continue;

			session->Send(sendBuffer);
		}
	}
}

void Region::Update()
{
	for (auto& player : _players)
		player.second->Update();

	for (auto& monster : _monsters)
		monster.second->Update();

	for (auto& projectile : _projectiles)
		projectile.second->Update();

	//	로직상 아직 플레이어업데이트 및 몬스터업데이트에서 서로의 HashMap을 건드리는 일은 없다.
	/*if (_removePlayersId.empty() == false)
	{
		for (auto id : _removePlayersId)
		{
			auto player = _players.find(id);
			if (player != _players.end())
				_players.erase(player);
		}

		_removePlayersId.clear();
	}

	if (_removeMonstersId.empty() == false)
	{
		for (auto id : _removeMonstersId)
		{
			auto monster = _monsters.find(id);
			if (monster != _monsters.end())
				_monsters.erase(monster);
		}
		_removeMonstersId.clear();
	}*/

	if (_removeProjectilesId.empty() == false)
	{
		for (auto id : _removeProjectilesId)
		{
			auto projectile = _projectiles.find(id);
			if (projectile != _projectiles.end())
				_projectiles.erase(projectile);
		}
		_removeProjectilesId.clear();
	}
}

void Region::Clear()
{
	for (auto& player : _players)
		player.second->Clear();

	for (auto& monster : _monsters)
		monster.second->SetRegion(nullptr);

	for (auto& projectile : _projectiles)
		projectile.second->SetRegion(nullptr);

	_players.clear();
	_monsters.clear();
	_projectiles.clear();
}

Zone* Region::FindZoneFromPosition(const Vector2& position, bool findAdjacentZone)
{
	auto fieldIndex = _field->FindFieldIndexFromPosition(position);
	if (fieldIndex.x < 0 || fieldIndex.y < 0)
	{
		if (findAdjacentZone)
			return nullptr;
		else
			ASSERT(false);
	}

	auto row = fieldIndex.y / _splitZoneCount;
	auto column = fieldIndex.x / _splitZoneCount;
	if (row < 0 || row >= _zones.size() || column < 0 || column >= _zones[0].size())
	{
		if (findAdjacentZone)
			return nullptr;
		else
			ASSERT(false);
	}

	return _zones[row][column].get();
}

Set<Zone*> Region::FindAdjacentZones(const Vector2& position, int32 detectionRange)
{
	Set<Zone*> zones;

	//	TEMP
	auto currentZone = FindZoneFromPosition(position);
	ASSERT(currentZone);

	zones.insert(currentZone);

	Vector2Int searchDir[] = {
		Vector2Int{ -detectionRange, -detectionRange},	//	LeftTop
		Vector2Int{				  0, -detectionRange},	//	Top
		Vector2Int{  detectionRange, -detectionRange},	//	RightTop
		Vector2Int{  detectionRange,               0},	//	Right
		Vector2Int{  detectionRange,  detectionRange},	//	RightBottom
		Vector2Int{               0,  detectionRange},	//	Botton
		Vector2Int{ -detectionRange,  detectionRange},	//	LeftBottom
		Vector2Int{ -detectionRange,               0},	//	Left
	};

	for (auto& dir : searchDir)
	{
		auto distance = dir * static_cast<int32>(MOVE_DISTANCE);
		auto destPosition = position + Vector2IntToVector2(distance);
		auto findZone = FindZoneFromPosition(destPosition, true);

		if (findZone)
			zones.insert(findZone);
	}

	return zones;
}

//	TODO : AStar
//	Monster Search Range = ..? 
//	.. 성능에 좀 그런데
std::shared_ptr<Player> Region::FindNearByPlayer(const Vector2& position, int32 searchRange)
{
	Vector<std::shared_ptr<Player>> players = std::move(FindPlayersFromAdjacentZones(position, searchRange));
	
	auto field = _field.get();
	std::sort(players.begin(), players.end(), [position, field](const std::shared_ptr<Player> p1, const std::shared_ptr<Player> p2)
		{
			auto p1Diff = Vector2ToVector2Int(p1->GetPosition() - position);
			auto p2Diff = Vector2ToVector2Int(p2->GetPosition() - position);

			auto p1Distance = abs(p1Diff.x) + abs(p1Diff.y);
			auto p2Distance = abs(p2Diff.x) + abs(p2Diff.y);

			return p1Distance < p2Distance;
		});

	for (auto& player : players)
	{
		auto diff = Vector2ToVector2Int(player->GetPosition() - position);
		auto distance = abs(diff.x) + abs(diff.y);
		if ((distance / TILE_DISTANCE) > searchRange)
			break;

		auto path = field->PathFinding(position, player->GetPosition(), true);
		//	거리가 너무 멀거나 길찾은게 이상하거나..
		if (path.size() < 2 || path.size() > searchRange)
			continue;

		return player;
	}

	return std::shared_ptr<Player>();
}

Vector<std::shared_ptr<Player>> Region::FindPlayersFromAdjacentZones(const Vector2& position, int32 searchRange)
{
	auto zones = FindAdjacentZones(position, searchRange);
	Vector<std::shared_ptr<Player>> players;
	
	for (auto& zone : zones)
	{
		for (auto& player : zone->GetPlayers())
			players.push_back(player);
	}

	return players;
}
