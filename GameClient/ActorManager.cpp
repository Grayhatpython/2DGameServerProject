#include "pch.h"
#include "ActorManager.h"
#include "Grid.h"
#include "Actor.h"
#include "MyPlayer.h"
#include "TransformComponent.h"

ActorManager* GActorManager = nullptr;

ActorManager::ActorManager()
{

}

ActorManager::~ActorManager()
{

}

void ActorManager::Update(float deltaTime)
{
	for (auto& actor : _actors)
		actor.second->Update(deltaTime);

	for (auto& effect : _effects)
		effect.second->Update(deltaTime);

	if(_camera)
		_camera->Update(deltaTime);
}

void ActorManager::LateUpdate()
{
	for (auto id : _deadActorsId)
	{
		auto iter = _actors.find(id);
		if (iter != _actors.end())
			_actors.erase(iter);
	}
	_deadActorsId.clear();

	for (auto& name : _removeEffectsId)
	{
		auto iter = _effects.find(name);
		if (iter != _effects.end())
			_effects.erase(iter);
	}
	_removeEffectsId.clear();

	/*
	//	삭제가 좀 느리다..
	std::list<std::unordered_map<int32, std::shared_ptr<Actor>>::const_iterator> deadActors;

	for (auto iter = _actors.cbegin(); iter != _actors.cend(); iter++)
	{
		if (iter->second->GetState() == ActorState::Dead)
			deadActors.push_back(iter);
	}

	for (auto& actor : deadActors) {
		_actors.erase(actor);
	}

	std::list<std::unordered_map<std::wstring, std::shared_ptr<Actor>>::const_iterator> deadEffects;

	for (auto iter = _effects.cbegin(); iter != _effects.cend(); iter++)
	{
		if (iter->second->GetState() == ActorState::Dead)
			deadEffects.push_back(iter);
	}

	for (auto& effect : deadEffects) {
		_effects.erase(effect);
	}
	*/
}

void ActorManager::Clear()
{
	_grid = nullptr;
	_camera = nullptr;
	_myPlayer = nullptr;
	_actors.clear();
	_effects.clear();
}

std::shared_ptr<Actor> ActorManager::FindActor(int32 id)
{
	auto iter = _actors.find(id);
	if (iter != _actors.end())
		return iter->second;

	return nullptr;
}

std::shared_ptr<Actor> ActorManager::FindActor(const Vector2& position)
{
	for (auto& actor : _actors)
	{
		auto transform = actor.second->GetComponent<TransformComponent>();
		if (transform == nullptr)
			continue;

		auto diff = transform->GetPosition() - position;
		auto distance = diff.Length();
		if (Math::NearZero(distance))
			return actor.second;
	}

	return std::shared_ptr<Actor>();
}

std::shared_ptr<Actor> ActorManager::FindActor(std::function<bool(const std::shared_ptr<Actor>&)> condition)
{
	for (auto& actor : _actors)
	{
		if (condition(actor.second))
			return actor.second;
	}

	return std::shared_ptr<Actor>();
}

void ActorManager::RemoveActors()
{
	_camera = nullptr;
	_myPlayer = nullptr;
	_actors.clear();
	//_effects.clear();
}

std::shared_ptr<Grid> ActorManager::AddGrid()
{
	std::shared_ptr<Actor> grid = std::make_shared<Grid>();
	_grid = grid;
	return std::static_pointer_cast<Grid>(grid);
}

std::shared_ptr<Grid> ActorManager::GetGrid()
{
	return std::static_pointer_cast<Grid>(_grid);
}

void ActorManager::RemoveMyPlayer()
{
	//	굳이..
	if (_myPlayer == nullptr)
		return;

	_myPlayer->SetState(ActorState::Dead);
	_myPlayer = nullptr;
}

std::shared_ptr<MyPlayer>	ActorManager::GetMyPlayer()
{
	return std::static_pointer_cast<MyPlayer>(_myPlayer);
}