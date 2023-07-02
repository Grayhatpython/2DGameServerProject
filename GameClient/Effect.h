#pragma once
#include "Actor.h"

class Effect : public Actor
{
public:
	Effect();
	virtual ~Effect();

public:
	virtual void	Initialize() override;
	virtual void	SpecificActorUpdate(float deltaTime);

protected:
	float			_liveTime = 0.5f;

private:
	static int32	_generateEffectId;
};

