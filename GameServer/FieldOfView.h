#pragma once

class Actor;
class Player;
//	�÷��̾��� �þ߰�
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
	//	������ �þ߰��ΰ�..
	std::weak_ptr<Player>		_owner;
	
	//	�������� Actor ���	
	//	Ref.. 
	//	Me?
	Set<std::shared_ptr<Actor>> _previouslySeenActors;
	uint64						_nextUpdateTick = 0;
};

