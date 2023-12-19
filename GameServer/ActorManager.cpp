#include "pch.h"
#include "ActorManager.h"
#include "Player.h"

ActorManager* GActorManager = nullptr;

ActorManager::ActorManager()
{

}

ActorManager::~ActorManager()
{
	Clear();
	std::cout << "~ActorManager" << std::endl;
}

void ActorManager::Clear()
{
	for (auto& player : _players)
		player.second->Clear();
}

void ActorManager::RemoveActor(int32 actorId)
{
	WRITE_LOCK;

	auto type = GetActorTypeFromId(actorId);
	if (type == Protocol::ActorType::PLAYER)
		_players.erase(actorId);
}

std::shared_ptr<Player> ActorManager::FindPlayerFromId(int32 actorId)
{
	WRITE_LOCK;
	auto type = GetActorTypeFromId(actorId);
	if (type == Protocol::ActorType::PLAYER)
	{
		auto iter = _players.find(actorId);
		if (iter != _players.end())
			return iter->second;
	}

	return std::shared_ptr<Player>();
}

//	추후 나간 플레이어 처리..