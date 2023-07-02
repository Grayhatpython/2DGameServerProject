#pragma once

class Actor;
class Player;
//	플레이어의 시야각
class FieldOfView 
{
public:
	FieldOfView();
	~FieldOfView();

public:
	void						Update();
	void						Clear();

public:
	void						SetOwner(std::weak_ptr<Player> owner) { _owner = owner; }
	std::shared_ptr<Player>		GetOwner() { return _owner.lock(); }

	Set<std::shared_ptr<Actor>>	GetNearByActors();

private:
	//	누구의 시야각인가..
	std::weak_ptr<Player>		_owner;
	
	//	보여지는 Actor 목록	
	//	Ref.. 
	//	Me?
	Set<std::shared_ptr<Actor>> _previouslySeenActors;
	uint64						_nextUpdateTick = 0;
};

