#pragma once

#include "Actor.h"

class Player;
class Monster;
class Projectile;
class Zone
{
public:
	Zone() = default;
	Zone(int32 row, int32 column);
	~Zone();
	
public:
	void								AddPlayer(std::shared_ptr<Player> player) { _players.insert(player); }
	void								RemovePlayer(std::shared_ptr<Player> player) { _players.erase(player); }

	void								AddMonster(std::shared_ptr<Monster> monsters) { _monsters.insert(monsters); }
	void								RemoveMonster(std::shared_ptr<Monster> monsters) { _monsters.erase(monsters); }

	void								AddProjectile(std::shared_ptr<Projectile> projectiles) { _projectiles.insert(projectiles); }
	void								RemoveProjectile(std::shared_ptr<Projectile> projectiles) { _projectiles.erase(projectiles); }

	void								AddActor(std::shared_ptr<Actor> actor);
	void								RemoveActor(std::shared_ptr<Actor> actor);

public:
	const Set<std::shared_ptr<Player>>&			GetPlayers() const { return _players; }
	const Set<std::shared_ptr<Monster>>&		GetMonsters() const { return _monsters; }
	const Set<std::shared_ptr<Projectile>>&		GetProjectiles() const { return _projectiles; }

	std::shared_ptr<Player>						FindPlayer(std::function<bool(const std::shared_ptr<Player>)> condition);
	Vector<std::shared_ptr<Player>>				FindPlayers(std::function<bool(const std::shared_ptr<Player>)> condition);

	int32										GetRow() { return _row; }
	int32										GetColumn() { return _column; }

private:
	//	룸에 들어오거나 나갈때 처리하자
	Set<std::shared_ptr<Player>>		_players;
	Set<std::shared_ptr<Monster>>		_monsters;
	Set<std::shared_ptr<Projectile>>	_projectiles;

	//	
	int32 _row = 0;
	int32 _column = 0;
};

