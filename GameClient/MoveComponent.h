#pragma once
#include "Component.h"

class MoveComponent : public Component
{
	COMPONENT_TYPE_DECLARATION(MoveComponent);

public:
	MoveComponent(int32 updateOrder = 10);
	virtual ~MoveComponent();

public:
	virtual void		Update(float deltaTime) override;

public:
	float				GetAngularSpeed() const { return _angularSpeed; }
	float				GetForwardSpeed() const { return _forwardSpeed; }
	float				GetDiagonalSpeed() const { return _diagonalSpeed; }

	void				SetAngularSpeed(float speed) { _angularSpeed = speed; }
	void				SetForwardSpeed(float speed) { _forwardSpeed = speed; }
	void				SetDiagonalSpeed(float speed) { _diagonalSpeed = speed; }

	Protocol::MoveDir	GetMoveDir() const { return _moveDir; }
	void				SetMoveDir(Protocol::MoveDir moveDir) { _moveDir = moveDir; }

	Vector2				GetMovePosition() const { return _movePosition; }
	void				SetMovePosition(const Vector2& position) { _movePosition = position; }

	Vector2				GetDestPosition() const { return _destPosition; }
	void				SetDestPosition(const Vector2& position) { _destPosition = position; }

	Protocol::MoveDir	GetMoveDirFromDirectionVector(const Vector2& vector);
	Vector2				GetForwardPositionFromMoveDir(Protocol::MoveDir moveDir);

	bool				IsDiagonalDirectionFromDirectionVector(const Vector2& vector);

protected:
	Protocol::MoveDir		_moveDir = Protocol::MoveDir::DOWN;
	Vector2					_movePosition{};
	Vector2					_destPosition{};

	float					_angularSpeed = 0.f;
	float					_diagonalSpeed = 0.f;
 	float					_forwardSpeed = 0.f;
};

