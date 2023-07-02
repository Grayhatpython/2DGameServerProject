#pragma once

class FSMComponent;
class AIState
{
public:
	AIState(const std::wstring& name);
	virtual ~AIState();

public:
	virtual void Initialize() abstract;
	virtual void Update(float deltaTime) abstract;
	virtual void UpdateAnimation();
	virtual void OnEnter() abstract;
	virtual void OnExit() abstract;

public:
	const std::wstring&				GetName() { return _name; }
	
	void							SetOwner(FSMComponent* owner) { _owner = owner; }
	FSMComponent*					GetOwner() { return _owner; }

private:
	std::wstring	_name;
	FSMComponent*	_owner = nullptr;
};

