#include "pch.h"
#include "Zone.h"

#include "Player.h"
#include "Monster.h"
#include "Projectile.h"

Zone::Zone(int32 row, int32 column)
	: _row(row) , _column(column)
{

}

Zone::~Zone()
{

}

void Zone::AddActor(std::shared_ptr<Actor> actor)
{
	switch (actor->GetType())
	{
	case Protocol::ActorType::PLAYER:
		{
			auto player = std::static_pointer_cast<Player>(actor);
			_players.insert(player);
		}
	break;
	case Protocol::ActorType::MONSTER:
		{
			auto monster = std::static_pointer_cast<Monster>(actor);
			_monsters.insert(monster);
		}
	break;
	case Protocol::ActorType::PROJECTILE:
		{
			auto projectile = std::static_pointer_cast<Projectile>(actor);
			_projectiles.insert(projectile);
		}
	break;
	}
}

void Zone::RemoveActor(std::shared_ptr<Actor> actor)
{
	switch (actor->GetType())
	{
	case Protocol::ActorType::PLAYER:
		{
			auto player = std::static_pointer_cast<Player>(actor);
			_players.erase(player);
		}
		break;
	case Protocol::ActorType::MONSTER:
		{
			auto monster = std::static_pointer_cast<Monster>(actor);
			_monsters.erase(monster);
		}
		break;
	case Protocol::ActorType::PROJECTILE:
		{
			auto projectile = std::static_pointer_cast<Projectile>(actor);
			_projectiles.erase(projectile);
		}
		break;
	}
}

std::shared_ptr<Player> Zone::FindPlayer(std::function<bool(const std::shared_ptr<Player>)> condition)
{
	for (auto& player : _players)
	{
		if (condition(player) == true)
			return player;
	}

	return nullptr;
}

Vector<std::shared_ptr<Player>> Zone::FindPlayers(std::function<bool(const std::shared_ptr<Player>)> condition)
{
	Vector<std::shared_ptr<Player>> players;

	for (auto& player : _players)
	{
		if (condition(player) == true)
			players.push_back(player);
	}

	return players;
}
