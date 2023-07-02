#pragma once
#include "AIState.h"

class Sweep;
class AIAutoAttackSkill : public AIState
{

public:
	AIAutoAttackSkill();
	virtual ~AIAutoAttackSkill();

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void UpdateAnimation();
	virtual void OnEnter() override;
	virtual void OnExit() override;

private:
	float			_coolDown = 0.f;
};

class AIBallAttackSkill : public AIState
{

public:
	AIBallAttackSkill();
	virtual ~AIBallAttackSkill();

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void UpdateAnimation();
	virtual void OnEnter() override;
	virtual void OnExit() override;

private:
	float		_coolDown = 0.f;
};

class AIMonsterAutoAttackSkill : public AIState
{

public:
	AIMonsterAutoAttackSkill();
	virtual ~AIMonsterAutoAttackSkill();

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void UpdateAnimation();
	virtual void OnEnter() override;
	virtual void OnExit() override;
};
