#include "pch.h"
#include "MoveComponent.h"
#include "Actor.h"
#include "MapManager.h"

COMPONENT_TYPE_DEFINITION(Component, MoveComponent);

MoveComponent::MoveComponent(int32 updateOrder)
	: Component(updateOrder)
{

}

MoveComponent::~MoveComponent()
{

}

void MoveComponent::Update(float deltaTime)
{

}

Protocol::MoveDir MoveComponent::GetMoveDirFromDirectionVector(const Vector2& vector)
{
	if (vector.x > 0 && vector.y > 0)
		return Protocol::MoveDir::DOWN_RIGHT;
	else if (vector.x < 0 && vector.y > 0)
		return Protocol::MoveDir::DOWN_LEFT;
	else if (vector.x > 0 && vector.y < 0)
		return Protocol::MoveDir::UP_RIGHT;
	else if (vector.x < 0 && vector.y < 0)
		return Protocol::MoveDir::UP_LEFT;
	else if (vector.x > 0)
		return Protocol::MoveDir::RIGHT;
	else if (vector.x < 0)
		return Protocol::MoveDir::LEFT;
	else if (vector.y > 0)
		return Protocol::MoveDir::DOWN;
	else /*if (vector.y < 0)*/
		return Protocol::MoveDir::UP;
}

Vector2 MoveComponent::GetForwardPositionFromMoveDir(Protocol::MoveDir moveDir)
{
	switch (moveDir)
	{
	case Protocol::MoveDir::UP:
		return Vector2{ 0.f, -MOVE_DISTANCE };
	case Protocol::MoveDir::UP_RIGHT:
		return Vector2{ MOVE_DISTANCE, -MOVE_DISTANCE };
	case Protocol::MoveDir::RIGHT:
		return Vector2{ MOVE_DISTANCE, 0.f };
	case Protocol::MoveDir::DOWN_RIGHT:
		return Vector2{ MOVE_DISTANCE, MOVE_DISTANCE };
	case Protocol::MoveDir::DOWN:
		return Vector2{ 0.f, MOVE_DISTANCE };
	case Protocol::MoveDir::DOWN_LEFT:
		return Vector2{ -MOVE_DISTANCE, MOVE_DISTANCE };
	case Protocol::MoveDir::LEFT:
		return Vector2{ -MOVE_DISTANCE, 0.f };
	case Protocol::MoveDir::UP_LEFT:
		return Vector2{ -MOVE_DISTANCE, -MOVE_DISTANCE };
	default:
		return Vector2{};
	}
}

bool MoveComponent::IsDiagonalDirectionFromDirectionVector(const Vector2& vector)
{
	auto moveDir = GetMoveDirFromDirectionVector(vector);

	if (moveDir == Protocol::UP_LEFT || moveDir == Protocol::UP_RIGHT || 
		moveDir == Protocol::DOWN_LEFT || moveDir == Protocol::DOWN_RIGHT)
		return true;

	return false;
}

