#include "pch.h"
#include "Effect.h"

int32	Effect::_generateEffectId = -1;

Effect::Effect()
{
	//	Effect Id 마이너스 
	_id = _generateEffectId--;
}

Effect::~Effect()
{
	
}

void Effect::Initialize()
{
	SetType(Protocol::ActorType::EFFECT);
}

void Effect::SpecificActorUpdate(float deltaTime)
{
	_liveTime -= deltaTime;
	if (_liveTime <= 0.f)
	{
		SetState(ActorState::Dead);
	}
}
