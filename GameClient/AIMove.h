#pragma once
#include "AIState.h"

class AIMove : public AIState
{

public:
	AIMove();
	virtual ~AIMove();

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void UpdateAnimation() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

public:
	void UpdatePosition(float deltaTime);
	void UpdateMovePosition();
};

class AIMonsterMove : public AIState
{

public:
	AIMonsterMove();
	virtual ~AIMonsterMove();

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void UpdateAnimation() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

public:
	void UpdatePosition(float deltaTime);

};

class AIProjectileMove : public AIState
{
public:
	AIProjectileMove();
	virtual ~AIProjectileMove();

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void UpdateAnimation() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

public:
	void UpdatePosition(float deltaTime);

};