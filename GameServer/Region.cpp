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

		//	���� ������..?
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

		//	���� ���..
		newPlayer->ReCalculationAdditionalStat();

		//	�ʵ忡 �÷��̾� ��ġ ���...
		{
			_field->RegisterActorsMappingTable(newPlayer);
		}

		{
			//	���ο� �÷��̾��� Ŭ���̾�Ʈ���� �뿡 ������ �Ϸ�����Ƿ� ���� �÷��̾� ���� Ŭ���̾�Ʈ���� ����
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

		//	�ʵ忡 ���� ��ġ ���
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

		//	������Ʈ �ָ��ϴ�..
		//newProjectile->Update();

		{
			//	ȭ���� Zone�� ����
			_field->RegisterActorsMappingTable(newProjectile);
		}
	}
	else
		ASSERT(false);

	{
		//	�ٸ� �÷��̾��� Ŭ���̾�Ʈ���� ��� ������ Actor�� �뿡 �����ߴٴ� ���� �˸���.
		//	���� �ڽ��� �÷��̾ �������� ��쿡�� ������ ENTER_GAME_PACKET���� ������ ������ ���⼭�� �����ȴ�.
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

			//	�ʵ忡�� ��ġ ��� ����
			_field->DeRegisterActorsMappingTable(leavePlayer, false);

			//	DB ���� ����
			leavePlayer->OnLeaveGame();

			//	Owner ���� ���� ���� ������ ������ �� �÷��̾ ��Ȱ���ϱ� ������ Onwer�� �ʱ�ȭ �����ʴ´�..
			//	�ʵ� �þ߰� ���� �ʱ�ȭ..
			auto fieldOfView = leavePlayer->GetFieldOfView();
			ASSERT(fieldOfView);
			fieldOfView->Clear();

			//	�� ����
			leavePlayer->SetRegion(nullptr);
			_players.erase(actorId);
			//_removePlayersId.push_back(actorId);

		}
		else
			ASSERT(false);

		{
			//	������ �� �ڽ��� �׾ �ٽ� �ٷ� EnterGame���� ��Ȱ���ֹǷ� ������ ������
			//	�������� ���� ������ �����ٴ� ��Ŷ�� ���� �ʿ䰡 ������??
			Protocol::S_LEAVE_GAME leavePacket;
			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(leavePacket);

			{	
				//	Disconnected ó��
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

			//	�ʵ忡�� ��ġ ��� ����
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

			//	ȭ���� Zone ������
			_field->DeRegisterActorsMappingTable(leaveProjectile, false);

			leaveProjectile->SetRegion(nullptr);
			_removeProjectilesId.push_back(actorId);
		}
		else
			ASSERT(false);
	}

	{
		//	�ٸ� �÷��̾�鿡�� �ش� Actor�� �������ٴ� ��Ŷ ����
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
		//	�������� ��ǥ �̵�?
		//	���� �̰� �̵� ��Ŷ�� ���� �ùķ��̼� ���ϰ� �ٷ� ��ǥ�� �̵��ع�����.
		//	TODO. ���������� Ŭ��� ���������� �ٷ� ��ǥ�̵���Ű�°� �ƴ϶� ������ ���鼭 �̵� ����ȭ ������Ѵ�.	
		//player->SetPositionInfo(movePacket->positioninfo());
		const auto& movePacketPositionInfo = movePacket.positioninfo();
		auto movePosition = Vector2{ movePacketPositionInfo.positionx(), movePacketPositionInfo.positiony() };
		auto playerPosition = player->GetPosition();

		//	�̵��� ����ϴ� ��ǥ�� ���� �̵��� ��û�� �÷��̾��� ��ġ�� ������ 
		//	���¸� ����Ǵ� ���
		if (Math::NearZero((movePosition - playerPosition).Length()) == false)
		{
			//	�ʵ忡�� Ÿ���� �̵� ���� ���ο� Ÿ�Ͽ� �÷��̾ �����ϴ� ��� ũ�ν� üũ
			if (_field->CanMovePosition(movePosition) == false)
				return;
		}

		{
			//	�̵����� �� ���
			//	���� ���������� ������ ������ ����
			//	�׷��� �̵� ������ �ٷ� ó���ȴ�.. ���Ŀ� ������Ʈ ����
			player->SetState(movePacketPositionInfo.state());
			player->SetMoveDir(movePacketPositionInfo.movedir());
			//	�ʵ忡�� �÷��̾� ���
			_field->ModifyActorsMappingTableByMove(player, movePosition);
		}
	}

	{
		//	�ٸ� �������� �ش� ������ �̵��ߴ� ���� ����
		//	�ٷ� �����°� ������ ? �����ϰ� �ٷ� ������ ?
		//	�������� �ù����̼� �Ŀ� �����ؼ� ������ ������ �� ���� �� ������..?
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

		//	Skill Logic ? ó�� ... 
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

//	���� �ش��ϴ� �÷��̾�鿡�Ը� BroadCast�� �Ѵ�..
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

	//	������ ���� �÷��̾������Ʈ �� ���;�����Ʈ���� ������ HashMap�� �ǵ帮�� ���� ����.
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
//	.. ���ɿ� �� �׷���
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
		//	�Ÿ��� �ʹ� �ְų� ��ã���� �̻��ϰų�..
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
