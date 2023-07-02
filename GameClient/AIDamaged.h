#pragma once
#include "AIState.h"

//	잠깐 UI와 Effect를 위해 들리는 상태 -> 서버 전송 x
class AIDamaged : public AIState
{

public:
	AIDamaged();
	virtual ~AIDamaged();

public:
	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;
	virtual void UpdateAnimation() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

private:
	float		_remainTime = 0.5f;
	const float _damagedStateRemainTime = 0.5f;
};
