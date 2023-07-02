#include "pch.h"
#include "FSMComponent.h"
#include "AIState.h"
#include "Protocol.pb.h"

#include "NetworkManager.h"
#include "ServerPacketHandler.h"

COMPONENT_TYPE_DEFINITION(Component, FSMComponent);

FSMComponent::FSMComponent()
{

}

FSMComponent::~FSMComponent()
{
	int a = 3;
}

void FSMComponent::Update(float deltaTime)
{
	if (_currentState)
		_currentState->Update(deltaTime);
}

//	Recv
void FSMComponent::ChangeState(const std::wstring& name)
{
	if (_currentState)
		_currentState->OnExit();

	auto iter = _states.find(name);
	if (iter != _states.end())
	{
		_currentState = iter->second.get();
		_currentState->OnEnter();
	}
	else
	{
		ASSERT(_currentState);
	}
}

void FSMComponent::ChangeState(const Protocol::AIState& state)
{
	if (_currentState)
		_currentState->OnExit();

	std::wstring name = FindStringFromAIState(state);

	auto iter = _states.find(name);
	if (iter != _states.end())
	{
		_currentState = iter->second.get();
		_currentState->OnEnter();
	}
	else
	{
		ASSERT(_currentState);
	}
}

//	Send
void FSMComponent::ChangeState(const Protocol::PositionInfo& info)
{
	auto owner = GetOwner();
	ASSERT(owner);

	if (_currentState)
		_currentState->OnExit();

	std::wstring name = FindStringFromAIState(info.state());

	{
		Protocol::C_MOVE movePacket;
		auto positionInfo = movePacket.mutable_positioninfo();
		positionInfo->CopyFrom(info);
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(movePacket);
		GNetworkManager->Send(sendBuffer);
	}

	auto iter = _states.find(name);
	if (iter != _states.end())
	{
		_currentState = iter->second.get();
		_currentState->OnEnter();
	}
	else
	{
		ASSERT(_currentState);
	}
}

const WCHAR* FSMComponent::FindStringFromAIState(const Protocol::AIState& state)
{
	switch (state)
	{
	case Protocol::AIState::IDLE:
		return  L"Idle";
	case Protocol::AIState::MOVE:
		return  L"Move";
	case Protocol::AIState::Skill:
		return  L"Skill";
	case Protocol::AIState::DEATH:
		return  L"Death";
	default:
		ASSERT(false);
		return L"";
	}
}

void FSMComponent::RemoveState(const std::wstring& name)
{
	auto iter = _states.find(name);
	if (iter != _states.end())
		_states.erase(iter);
}
