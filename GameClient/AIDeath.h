#pragma once
#include "AIState.h"

class Explosion;
class AIDeath : public AIState
{
public:
	AIDeath();
	virtual ~AIDeath();

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void UpdateAnimation() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

};
