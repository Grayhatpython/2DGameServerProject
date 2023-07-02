#pragma once
#include "Component.h"

class TransformComponent : public Component
{
	COMPONENT_TYPE_DECLARATION(TransformComponent);

public:
	TransformComponent();
	virtual ~TransformComponent();

public:
	const Vector2&		GetPosition() const { return _position; }
	void				SetPosition(const Vector2& position) { _position = position; }

	float				GetScale() const { return _scale; }
	void				SetScale(float scale) { _scale = scale; }

	float				GetRotation() const { return _rotation; }
	void				SetRotation(float rotation) { _rotation = rotation; }

	Vector2				GetForwardDir() const { return Vector2(Math::Cos(_rotation), -Math::Sin(_rotation)); }
	Vector2				GetForwardPosition(Protocol::MoveDir moveDir)
	{
		Vector2 forwardPosition;
		switch (moveDir)
		{
		case Protocol::MoveDir::UP:
			forwardPosition.y = -MOVE_DISTANCE;
			break;
		case Protocol::MoveDir::UP_RIGHT:
			forwardPosition.x = MOVE_DISTANCE;
			forwardPosition.y = -MOVE_DISTANCE;
			break;
		case Protocol::MoveDir::RIGHT:
			forwardPosition.x = MOVE_DISTANCE;
			break;
		case Protocol::MoveDir::DOWN_RIGHT:
			forwardPosition.x = MOVE_DISTANCE;
			forwardPosition.y = MOVE_DISTANCE;
			break;
		case Protocol::MoveDir::DOWN:
			forwardPosition.y = MOVE_DISTANCE;
			break;
		case Protocol::MoveDir::DOWN_LEFT:
			forwardPosition.x = -MOVE_DISTANCE;
			forwardPosition.y = MOVE_DISTANCE;
			break;
		case Protocol::MoveDir::LEFT:
			forwardPosition.x = -MOVE_DISTANCE;
			break;
		case Protocol::MoveDir::UP_LEFT:
			forwardPosition.x = -MOVE_DISTANCE;
			forwardPosition.y = -MOVE_DISTANCE;
			break;
		}

		return forwardPosition + _position;
	}

private:
	Vector2				_position{};
	float				_scale = 1.f;
	float				_rotation = 0.f;

};

