#include "pch.h"
#include "FieldOfView.h"
#include "Region.h"
#include "Player.h"
#include "Monster.h"
#include "Projectile.h"
#include "ClientSession.h"
#include "ClientPacketHandler.h"

FieldOfView::FieldOfView()
{

}

FieldOfView::~FieldOfView()
{

}

void FieldOfView::Update()
{
	if (_nextUpdateTick > ::GetTickCount64())
		return;

	_nextUpdateTick = ::GetTickCount64() + 200;

	auto owner = GetOwner();
	ASSERT(owner);
	auto region = owner->GetRegion();
	ASSERT(region);
	auto field = region->GetField();
	ASSERT(field);

	auto currentSeenActors = GetNearByActors();

	//	기존의 시야각에서 새로 생긴 엑터는 Spawn
	Set<std::shared_ptr<Actor>> spawnActors;
	std::set_difference(currentSeenActors.begin(), currentSeenActors.end(), _previouslySeenActors.begin(), _previouslySeenActors.end(),
		std::inserter(spawnActors, spawnActors.end()));
	if (spawnActors.size() > 0)
	{
		Protocol::S_SPAWN spawnSendPacket;

		for (auto& actor : spawnActors)
		{
			auto addActor = spawnSendPacket.add_actors();
			addActor->MergeFrom(actor->GetInfo());
		}

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(spawnSendPacket);
		owner->GetSession()->Send(sendBuffer);
	}

	//	기존의 시야각에서 없어진 엑터는 Despawn
	Set<std::shared_ptr<Actor>> despawnActors;
	std::set_difference(_previouslySeenActors.begin(), _previouslySeenActors.end(), currentSeenActors.begin(), currentSeenActors.end(),
		std::inserter(despawnActors, despawnActors.end()));
	if (despawnActors.size() > 0)
	{
		Protocol::S_DESPAWN despawnSendPacket;

		for (auto& actor : despawnActors)
			despawnSendPacket.add_actorid(actor->GetId());

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(despawnSendPacket);
		owner->GetSession()->Send(sendBuffer);
	}

	_previouslySeenActors = std::move(currentSeenActors);
}

void FieldOfView::Clear()
{
	_previouslySeenActors.clear();
}

Set<std::shared_ptr<Actor>> FieldOfView::GetNearByActors()
{
	auto owner = GetOwner();
	ASSERT(owner);
	auto region = owner->GetRegion();
	ASSERT(region);
	auto field = region->GetField();
	ASSERT(field);

	auto detectionRange = region->GetDectionRange();

	Set<std::shared_ptr<Actor>> actors;

	auto position = owner->GetPosition();
	auto adjacentZones = region->FindAdjacentZones(position);

	for (auto zone : adjacentZones)
	{
		for (auto& player : zone->GetPlayers())
		{
			if (owner == player)
				continue;

			auto playerTileIndex = field->FindFieldIndexFromPosition(player->GetPosition());
			ASSERT(playerTileIndex.x >= 0 || playerTileIndex.y >= 0);
			auto ownerTileIndex = field->FindFieldIndexFromPosition(position);
			ASSERT(ownerTileIndex.x >= 0 || ownerTileIndex.y >= 0);

			auto diff = playerTileIndex - ownerTileIndex;
			if (std::abs(diff.x) > detectionRange)
				continue;
			if (std::abs(diff.y) > detectionRange)
				continue;

			actors.insert(player);
		}

		for (auto& monster : zone->GetMonsters())
		{
			auto monsterTileIndex = field->FindFieldIndexFromPosition(monster->GetPosition());
			ASSERT(monsterTileIndex.x >= 0 || monsterTileIndex.y >= 0);
			auto ownerTileIndex = field->FindFieldIndexFromPosition(position);
			ASSERT(ownerTileIndex.x >= 0 || ownerTileIndex.y >= 0);

			auto diff = monsterTileIndex - ownerTileIndex;
			if (std::abs(diff.x) > detectionRange)
				continue;
			if (std::abs(diff.y) > detectionRange)
				continue;

			actors.insert(monster);
		}

		for (auto& projectile : zone->GetProjectiles())
		{
			auto projectileTileIndex = field->FindFieldIndexFromPosition(projectile->GetPosition());
			ASSERT(projectileTileIndex.x >= 0 || projectileTileIndex.y >= 0);
			auto ownerTileIndex = field->FindFieldIndexFromPosition(position);
			ASSERT(ownerTileIndex.x >= 0 || ownerTileIndex.y >= 0);

			auto diff = projectileTileIndex - ownerTileIndex;
			if (std::abs(diff.x) > detectionRange)
				continue;
			if (std::abs(diff.y) > detectionRange)
				continue;

			actors.insert(projectile);
		}
	}

	return actors;
}
