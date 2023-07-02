#pragma once
#include "AIState.h"

class AIIdle : public AIState
{

public:
	AIIdle();
	virtual ~AIIdle();

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void UpdateAnimation() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

private:
	//	TEMP
	bool	_isSkillKeyDown = false;
};

class AIMonsterIdle : public AIState
{

public:
	AIMonsterIdle();
	virtual ~AIMonsterIdle();

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void UpdateAnimation() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

};