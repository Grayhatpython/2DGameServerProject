#pragma once
#include "Component.h"

class CameraComponent : public Component
{
	COMPONENT_TYPE_DECLARATION(CameraComponent);

public:
	CameraComponent(int32 updateOrder = 200);
	virtual ~CameraComponent();

public:
	virtual void Update(float deltaTime) override;

public:
	void					SetTarget(std::weak_ptr<Actor> target) { _target = target; }
	std::shared_ptr<Actor>	GetTarget() { return _target.lock(); }

private:
	std::weak_ptr<Actor>	_target;
	//	정중앙 기준 카메라 이동
	//	스크린 좌표
	//	월드 좌표
	//	타켓 위치
};
