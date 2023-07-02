#pragma once
#include "Player.h"

//	귓속말이나 특정 플레이어 대상으로 하고 싶은 컨텐츠를 위해
class ActorManager
{
public:
	ActorManager();
	~ActorManager();

public:
	void						Clear();

public:
	template<typename T>
	std::shared_ptr<T>			AddActor()
	{
		WRITE_LOCK;
		static_assert(std::is_base_of<Actor, T>::value, "Not Actor");
		auto actor = ObjectPool<T>::MakeShared();
		ASSERT(actor);

		auto type = actor->GetType();
		auto id = GenerateIdFromActorType(type);
		actor->SetId(id);

		if (type == Protocol::ActorType::PLAYER)
		{
			actor->Initialize();
			AddPlayer(actor);
		}

		return actor;
	}

	void						RemoveActor(int32 actorId);
	std::shared_ptr<Player>		FindPlayerFromId(int32 actorId);

public:
	static Protocol::ActorType	GetActorTypeFromId(int32 id)
	{
		int32 type = (id >> 24) & 0x7F;
		return static_cast<Protocol::ActorType>(type);
	}

private:
	int32 GenerateIdFromActorType(Protocol::ActorType type)
	{
		int32 id = static_cast<int32>(type) << 24 | _generateId;
		_generateId++;
		return id;
	}

private:
	void AddPlayer(std::shared_ptr<Actor> actor)
	{
		auto player = std::static_pointer_cast<Player>(actor);
		_players.insert(std::make_pair(actor->GetId(), player));
	}

private:
	const ActorManager& operator = (const ActorManager&) = delete;
	ActorManager(const ActorManager&) = delete;

private:
	USE_LOCK;	
	//	음.. 일단 들어오는대로 나가는거에 상관없이 서버 종료전까지 계속 쌓이긴함
	HashMap<int32, std::shared_ptr<Player>>		_players;
	int32										_generateId = 1;
};

extern ActorManager* GActorManager;