#pragma once

#include "Actor.h"
#include "MoveComponent.h"
#include "TransformComponent.h"

class Grid;
class MyPlayer;
class ActorManager
{
public:
	ActorManager();
	~ActorManager();

public:
	void Update(float deltaTime);
	void LateUpdate();
	void Clear();

public:
	template<typename T>
	std::shared_ptr<T> AddActor(int32 id)
	{
		std::shared_ptr<Actor> pushActor = ObjectPool<T>::MakeShared();
		_actors.insert(std::make_pair(id, pushActor));
		return std::static_pointer_cast<T>(pushActor);
	}

	template<typename T>
	std::shared_ptr<T> AddActor(const Protocol::ActorInfo& info, int32 id)
	{
		std::shared_ptr<Actor> pushActor = ObjectPool<T>::MakeShared();

		std::wstring name{ UtilityHelper::ConvertUTF8ToUnicode(info.name()) };

		Vector2 position{};
		position.x = info.positioninfo().positionx();
		position.y = info.positioninfo().positiony();
		auto type = ActorManager::GetActorTypeFromId(id);

		pushActor->SetType(type);
		pushActor->Initialize();

		auto transform = pushActor->GetComponent<TransformComponent>();
		pushActor->SetId(id);
		pushActor->SetName(name);
		transform->SetPosition(position);

		auto move = pushActor->GetComponent<MoveComponent>();
		move->SetMoveDir(info.positioninfo().movedir());
		move->SetForwardSpeed(info.statinfo().speed() * MOVE_DISTANCE);
		move->SetDiagonalSpeed(info.statinfo().speed()* DIAGONAL_MOVE_DISTANCE);

		_actors.insert(std::make_pair(id, pushActor));
		return std::static_pointer_cast<T>(pushActor);
	}

	void RemoveActor(int32 id)
	{
		auto iter = _actors.find(id);
		if (iter != _actors.end())
			_actors.erase(iter);
	}

	template<typename T>
	std::shared_ptr<T> AddEffect()
	{
		std::shared_ptr<Actor> effect = ObjectPool<T>::MakeShared();
		effect->Initialize();
		_effects[effect->GetId()] = effect;
		return std::static_pointer_cast<T>(effect);
	}

public:
	static Protocol::ActorType	GetActorTypeFromId(int32 id)
	{
		int32 type = (id >> 24) & 0x7F;
		return static_cast<Protocol::ActorType>(type);
	}

public:
	std::shared_ptr<Actor> FindActor(int32 id);
	std::shared_ptr<Actor> FindActor(const Vector2& position);
	std::shared_ptr<Actor> FindActor(std::function<bool(const std::shared_ptr<Actor>&)> condition);

public:
	void AddDeatActorId(int32 id) { _deadActorsId.push_back(id); }
	void AddRemoveEffectId(int32 id) { _removeEffectsId.push_back(id); }

public:
	std::shared_ptr<Grid>									AddGrid();
	std::shared_ptr<Grid>									GetGrid();

	const	HashMap<int32, std::shared_ptr<Actor>>&			GetActors() { return _actors; }
	const	HashMap<int32, std::shared_ptr<Actor>>&			GetEffects() { return _effects; }
	
	void													SetCamera(std::shared_ptr<Actor> camera) { _camera = camera; }
	void													SetMyPlayer(std::shared_ptr<Actor> myPlayer) { _myPlayer = myPlayer; }

	std::shared_ptr<MyPlayer>								GetMyPlayer();
	void													RemoveMyPlayer();

	void													RemoveActors();

private:
	const ActorManager& operator = (const ActorManager&) = delete;
	ActorManager(const ActorManager&) = delete;

private:
	std::shared_ptr<Actor>						_grid;
	std::shared_ptr<Actor>						_camera;
	std::shared_ptr<Actor>						_myPlayer;
	HashMap<int32, std::shared_ptr<Actor>>		_actors;
	HashMap<int32, std::shared_ptr<Actor>>		_effects;

	std::vector<int32>							_deadActorsId;
	std::vector<int32>							_removeEffectsId;
};


extern ActorManager* GActorManager;

