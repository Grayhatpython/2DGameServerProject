#pragma once
#include "Protocol.pb.h"

#define MOVE_DISTANCE 32.f

class MyPlayer
{

public:
	void						SetPositionInfo(const Protocol::PositionInfo& positionInfo) { _positionInfo.MergeFrom(positionInfo); }
	const						Protocol::PositionInfo& GetPositionInfo() { return _positionInfo; }
	Protocol::MoveDir			GetMoveDir() { return _positionInfo.movedir(); }
	Vector2	GetForwardPosition(Protocol::MoveDir moveDir)
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

		auto position = Vector2{ _positionInfo.positionx(), _positionInfo.positiony() };
		return forwardPosition + position;
	}

private:
	Protocol::PositionInfo		_positionInfo;

};

