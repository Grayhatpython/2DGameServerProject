#include "pch.h"
#include "Monster.h"
#include "Region.h"
#include "Item.h"
#include "Field.h"
#include "Player.h"
#include "Random.h"
#include "Inventory.h"
#include "DataManager.h"
#include "RegionManager.h"
#include "RegionProcess.h"
#include "ClientPacketHandler.h"

#include "DBProcess.h"
Monster::Monster()
	: Actor(Protocol::ActorType::MONSTER)
{

}

Monster::~Monster()
{
	std::wcout << "~Monster" << std::endl;
}

void Monster::Initialize()
{
	//	TEMP
	_dataSheetId = 1;

	const auto& monsterData = DataManager::FindMonsterData(1);
	SetStatInfo(monsterData.statInfo);
	SetHp(monsterData.statInfo.maxhp());

	SetMoveDir(Protocol::MoveDir::DOWN);
	SetState(Protocol::AIState::IDLE);
	SetUsedSkillId(Protocol::SkillType::SKILL_NONE);

}

void Monster::Update(float deltaTime)
{
	auto state = GetState();
	switch (state)
	{
		case Protocol::IDLE:
			UpdateIdle();
			break;
		case Protocol::MOVE:
			UpdateMove();
			break;
		case Protocol::Skill:
			UpdateSkill();
			break;
		case Protocol::DEATH:
			UpdateDeath();
			break;
	}
}

void Monster::OnDamaged(std::shared_ptr<Actor> attacker, int32 damage)
{
	Actor::OnDamaged(attacker, damage);
}

void Monster::OnDead(std::shared_ptr<Actor> attacker)
{
	Actor::OnDead(attacker);

	auto target = GetTargetPlayer();
	if (target)
		_target = std::shared_ptr<Player>();

	auto owner = attacker->GetOwner();
	ASSERT(owner);

	if (owner->GetType() == Protocol::ActorType::PLAYER)
	{
		auto rewardData = GetRandomRewardData();
		if (rewardData.itemId > 0)
		{
			auto player = std::static_pointer_cast<Player>(owner);
			ASSERT(player);

			auto inventory = player->GetInventory();
			ASSERT(inventory);
			
			GDBProcess->InsertRewardItemToPlayer(player, rewardData);
			//	....
		}
	}
}

void Monster::ModifyMoveInfo()
{
	auto field = _region->GetField();
	ASSERT(field);

	/*auto regionProcess = GRegionManager->GetRegionProcess();
	regionProcess->OnlyPushJobAndNotDistribute([field, this, path = path[1]]() {
			field->ModifyActorsMappingTableByMove(shared_from_this(), path);
		});*/

	field->ModifyActorsMappingTableByMove(shared_from_this(), _movePath);

	{
		//	다음 목적지로 이동
		Protocol::S_MOVE moveSendPacket;
		moveSendPacket.set_actorid(GetId());
		moveSendPacket.mutable_positioninfo()->MergeFrom(GetPositionInfo());
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(moveSendPacket);
		_region->BroadCast(GetPosition(), sendBuffer);
	}

	_needToMove = false;
	_movePath = {};
}

void Monster::UseSkill()
{
	Protocol::S_SKILL skillSendPacket;
	SetUsedSkillId(Protocol::SkillType::SKILL_AUTO_ATTACK);
	skillSendPacket.set_actorid(GetId());
	skillSendPacket.mutable_skillinfo()->set_skillid(Protocol::SkillType::SKILL_AUTO_ATTACK);
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(skillSendPacket);
	_region->BroadCast(GetPosition(), sendBuffer);

	const auto& skillData = DataManager::FindSkillData(Protocol::SkillType::SKILL_AUTO_ATTACK);
	auto target = GetTargetPlayer();
	//	Target이 nullptr?
	target->OnDamaged(shared_from_this(), GetTotalAttack() + skillData.damage);

	_needUseSkill = false;
}

void Monster::UpdateIdle()
{
	if (_nextSearchTick > ::GetTickCount64())
		return;

	//	TEMP
	_nextSearchTick = ::GetTickCount64() + 1500;

	/*
	* 	auto targetPlayer = _room->FindNearByPlayer(GetPosition(), _numberOfTilesToSearch);

	if (targetPlayer == nullptr)
		return;
	*/

	//	AStar 개선
	//	현재 타켓팅 하고 있는 플레이어가 죽으면 
	//	그 타켓을 풀어주지않아서 그 타켓이 서치 범위 외에서 태어나면 계속 추적이 실패해서 
	//	옆에 다른 플레이어들이 있어도 가만히 있는 상황 발생!
	/*if (GetTargetPlayer() == nullptr)
	{
		std::shared_ptr<Player> targetPlayer;
		const auto& players = _room->GetPlayers();
		for (const auto& player : players)
		{
			auto numberOfTilesToMove = _room->GetField()->GetHowManyTilesAwayFromHereToThere(GetPosition(), player.second->GetPosition());
			if (_numberOfTilesToSearch > numberOfTilesToMove)
			{
				targetPlayer = player.second;
				break;
			}
		}

		if (targetPlayer == nullptr)
			return;

		_target = targetPlayer;
	}*/

	auto targetPlayer = _region->FindNearByPlayer(GetPosition(), _numberOfTilesToSearch);
	if (targetPlayer == nullptr)
		return;

	_target = targetPlayer;

	SetState(Protocol::AIState::MOVE);
}

void Monster::UpdateMove()
{
	if (_nextMoveTick > ::GetTickCount64())
		return;

	//	TEMP
	auto waitTick = 250;
	_nextMoveTick = ::GetTickCount64() + waitTick;

	auto target = GetTargetPlayer();
	//	내가 쫒아가고 있는 타켓이 게임 접속을 종료하거나 ,, 다른 필드로 이동한경우
	if (target == nullptr || target->GetRegion() == nullptr)
	{
		_target = std::shared_ptr<Player>();
		SetState(Protocol::AIState::IDLE);
		BroadCastChangeState(Protocol::AIState::IDLE);
		return;
	}

	auto field = _region->GetField();
	ASSERT(field);

	auto numberOfTilesToMove = field->GetHowManyTilesAwayFromHereToThere(target->GetPosition(), GetPosition());
	//	몬스터의 추격 범위보다 타켓이 더 멀어진 경우
	if(numberOfTilesToMove == 0 || numberOfTilesToMove > _numberOfTilesToChase)
	{
		_target = std::shared_ptr<Player>();
		SetState(Protocol::AIState::IDLE);
		BroadCastChangeState(Protocol::AIState::IDLE);
		return;
	}

	auto path = field->PathFinding(GetPosition(), target->GetPosition(), true);
	//	거리가 너무 멀거나 길찾은게 이상하거나..
	if (path.size() < 2 || path.size() > _numberOfTilesToChase)
	{
		_target = std::shared_ptr<Player>();
		SetState(Protocol::AIState::IDLE);
		BroadCastChangeState(Protocol::AIState::IDLE);
		return;
	}

	//	스킬 사정거리 체크
	if (numberOfTilesToMove <= _numberOfTilesToSkill)
	{
		_nextUseSkillTick = 0;
		SetState(Protocol::AIState::Skill);
		return;
	}
	
	auto directionVector = path[1] - GetPosition();

	//	이동 방향
	auto moveDir = GetMoveDirFromDirectionVector(directionVector);
	SetMoveDir(moveDir);

	//	필드에 맵핑 테이블에 다음 위치를 기반으로 등록 및 위치 이동
	//	동시 스레드 ...;;

	field->ModifyActorsMappingTableByMove(shared_from_this(), path[1]);

	{
		//	다음 목적지로 이동
		Protocol::S_MOVE moveSendPacket;
		moveSendPacket.set_actorid(GetId());
		moveSendPacket.mutable_positioninfo()->MergeFrom(GetPositionInfo());
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(moveSendPacket);
		_region->BroadCast(GetPosition(), sendBuffer);
	}

	/*_needToMove = true;

	_movePath = path[1];*/

}

void Monster::UpdateSkill()
{
	//	스킬 가능 상태
	if (_nextUseSkillTick == 0)
	{
		auto target = GetTargetPlayer();
		//	타켓이 설정되어 있는지 , 같은 게임 방에 있는지 , 같은 필드에 있는지 , 타켓이 체력이 0이 아닌지
		if (target == nullptr || target->GetRegion() != _region ||
			target->GetRegion()->GetField() != _region->GetField() || target->GetHp() == 0)
		{
			//	패킷 하나 만들자 상태만 변경하는
			_target = std::shared_ptr<Player>();
			SetState(Protocol::AIState::IDLE);
			BroadCastChangeState(Protocol::AIState::IDLE);
			return;
		}

		{
			auto field = _region->GetField();
			ASSERT(field);
			auto numberOfTilesToMove = field->GetHowManyTilesAwayFromHereToThere(target->GetPosition(), GetPosition());
			//	타켓이 스킬 범위를 벗어 낫는지
			if (numberOfTilesToMove > _numberOfTilesToSkill)
			{
				SetState(Protocol::AIState::MOVE);
				BroadCastChangeState(Protocol::AIState::MOVE);
				return;
			}
		}

		{
			auto directionVector = target->GetPosition() - GetPosition();

			//	이동 방향
			auto lookAt = GetMoveDirFromDirectionVector(directionVector);
			if (GetMoveDir() != lookAt)
			{
				SetMoveDir(lookAt);
				Protocol::S_CHANGE_MOVE_DIR changeMoveDirSendPacket;
				changeMoveDirSendPacket.set_actorid(GetId());
				changeMoveDirSendPacket.set_movedir(lookAt);
				auto sendBuffer = ClientPacketHandler::MakeSendBuffer(changeMoveDirSendPacket);
				_region->BroadCast(GetPosition(), sendBuffer);
			}
		}

		{
			//_needUseSkill = true;
	
			Protocol::S_SKILL skillSendPacket;
			SetUsedSkillId(Protocol::SkillType::SKILL_AUTO_ATTACK);
			skillSendPacket.set_actorid(GetId());
			skillSendPacket.mutable_skillinfo()->set_skillid(Protocol::SkillType::SKILL_AUTO_ATTACK);
			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(skillSendPacket);
			_region->BroadCast(GetPosition(), sendBuffer);

			const auto& skillData = DataManager::FindSkillData(Protocol::SkillType::SKILL_AUTO_ATTACK);
			target->OnDamaged(shared_from_this(), GetTotalAttack() + skillData.damage);

			/*auto regionProcess = GRegionManager->GetRegionProcess();
			regionProcess->OnlyPushJobAndNotDistribute([target, this, damage = skillData.damage]() {
					target->OnDamaged(shared_from_this(), GetTotalAttack() + damage);
				});*/

			uint64 nextUseSkillTick = static_cast<uint64>(6000 * skillData.coolDown);
			_nextUseSkillTick = ::GetTickCount64() + nextUseSkillTick;
		}
	}

	if (_nextUseSkillTick > ::GetTickCount64())
		return;

	_nextUseSkillTick = 0;
	SetState(Protocol::AIState::IDLE);
	//SetUsedSkillId(Protocol::SkillType::SKILL_NONE);
	BroadCastChangeState(Protocol::AIState::IDLE);
}

void Monster::UpdateDeath()
{

}

void Monster::BroadCastChangeState(const Protocol::AIState& state)
{
	Protocol::S_CHANGE_STATE changeStateSendPacket;
	changeStateSendPacket.set_actorid(GetId());
	changeStateSendPacket.set_state(state);
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(changeStateSendPacket);
	_region->BroadCast(GetPosition(), sendBuffer);
}

GameData::RewardData Monster::GetRandomRewardData()
{
	const auto& monsterData = DataManager::FindMonsterData(_dataSheetId);
	
	//	0 ~ 100 randValue
	int32 sum = 0;
	int32 randValue = Random::GetIntRange(0, 100);

	//	5 5 5 10
	for (auto& rewardData : monsterData.rewardDatas)
	{
		sum += rewardData.probability;

		if (randValue <= sum)
			return rewardData;
	}

	return GameData::RewardData{};
}
