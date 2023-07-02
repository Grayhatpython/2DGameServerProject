#include "pch.h"
#include "AIDamaged.h"
#include "FSMComponent.h"
#include "MoveComponent.h"
#include "StatComponent.h"
#include "GUIComponent.h"
#include "TransformComponent.h"
#include "PlayerInputComponent.h"
#include "AnimSpriteComponent.h"

#include "Ball.h"
#include "Actor.h"
#include "MapManager.h"
#include "AssetManager.h"
#include "ActorManager.h"

#include "Hit.h"
#include "HealthBar.h"

AIDamaged::AIDamaged()
	: AIState(L"Damaged")
{
	
}

AIDamaged::~AIDamaged()
{

}

void AIDamaged::Initialize()
{
	auto fsm = GetOwner();
	ASSERT(fsm);
	auto owner = fsm->GetOwner();
	ASSERT(owner);

	auto sprite = owner->GetComponent<AnimSpriteComponent>();
	ASSERT(sprite);

	auto texture = GAssetManager->GetTexture("../Assets/Player/Player_Move.png");
	AnimSpriteInfo animInfo;
	animInfo.texture = texture;
	animInfo.width = 32;
	animInfo.height = 32;
	animInfo.animDir = AnimSpriteDirection::Row;
	animInfo.repeat = false;

	{
		animInfo.maxFrameCount = 2;
		animInfo.standardXIndex = 13;
		animInfo.standardYIndex = 2;
		animInfo.flip = SDL_FLIP_NONE;
		sprite->AddAnimInfo(L"PLAYER_DAMAGED_RIGHT", animInfo);
	}

	{
		animInfo.maxFrameCount = 2;
		animInfo.standardXIndex = 13;
		animInfo.standardYIndex = 2;
		animInfo.flip = SDL_FLIP_HORIZONTAL;
		sprite->AddAnimInfo(L"PLAYER_DAMAGED_LEFT", animInfo);
	}

	{
		animInfo.maxFrameCount = 2;
		animInfo.standardXIndex = 13;
		animInfo.standardYIndex = 0;
		animInfo.flip = SDL_FLIP_NONE;
		sprite->AddAnimInfo(L"PLAYER_DAMAGED_UP", animInfo);
	}

	{
		animInfo.maxFrameCount = 2;
		animInfo.standardXIndex = 13;
		animInfo.standardYIndex = 4;
		animInfo.flip = SDL_FLIP_NONE;
		sprite->AddAnimInfo(L"PLAYER_DAMAGED_DOWN", animInfo);
	}

	{
		animInfo.maxFrameCount = 2;
		animInfo.standardXIndex = 13;
		animInfo.standardYIndex = 1;
		animInfo.flip = SDL_FLIP_NONE;
		sprite->AddAnimInfo(L"PLAYER_DAMAGED_UP_RIGHT", animInfo);
	}

	{
		animInfo.maxFrameCount = 2;
		animInfo.standardXIndex = 13;
		animInfo.standardYIndex = 1;
		animInfo.flip = SDL_FLIP_HORIZONTAL;
		sprite->AddAnimInfo(L"PLAYER_DAMAGED_UP_LEFT", animInfo);
	}

	{
		animInfo.maxFrameCount = 2;
		animInfo.standardXIndex = 13;
		animInfo.standardYIndex = 3;
		animInfo.flip = SDL_FLIP_NONE;
		sprite->AddAnimInfo(L"PLAYER_DAMAGED_DOWN_RIGHT", animInfo);
	}

	{
		animInfo.maxFrameCount = 2;
		animInfo.standardXIndex = 13;
		animInfo.standardYIndex = 3;
		animInfo.flip = SDL_FLIP_HORIZONTAL;
		sprite->AddAnimInfo(L"PLAYER_DAMAGED_DOWN_LEFT", animInfo);
	}
}

void AIDamaged::Update(float deltaTime)
{
	_remainTime -= deltaTime;
	if (_remainTime <= 0.f)
	{

		auto fsm = GetOwner();
		ASSERT(fsm);

		fsm->ChangeState(Protocol::AIState::IDLE);
	}
}

void AIDamaged::UpdateAnimation()
{
	auto fsm = GetOwner();
	ASSERT(fsm);
	if (fsm->GetCurrentStateName().compare(L"Damaged") != 0)
		return;

	auto owner = fsm->GetOwner();
	ASSERT(owner);

	auto sprite = owner->GetComponent<AnimSpriteComponent>();
	ASSERT(sprite);

	sprite->SetAnimFPS(3.f);

	auto move = owner->GetComponent<MoveComponent>();
	ASSERT(move);

	auto moveDir = move->GetMoveDir();

	switch (moveDir)
	{
	case Protocol::MoveDir::UP:
		sprite->Play(L"PLAYER_DAMAGED_UP");
		break;
	case Protocol::MoveDir::UP_RIGHT:
		sprite->Play(L"PLAYER_DAMAGED_UP_RIGHT");
		break;
	case Protocol::MoveDir::RIGHT:
		sprite->Play(L"PLAYER_DAMAGED_RIGHT");
		break;
	case Protocol::MoveDir::DOWN_RIGHT:
		sprite->Play(L"PLAYER_DAMAGED_DOWN_RIGHT");
		break;
	case Protocol::MoveDir::DOWN:
		sprite->Play(L"PLAYER_DAMAGED_DOWN");
		break;
	case Protocol::MoveDir::DOWN_LEFT:
		sprite->Play(L"PLAYER_DAMAGED_DOWN_LEFT");
		break;
	case Protocol::MoveDir::LEFT:
		sprite->Play(L"PLAYER_DAMAGED_LEFT");
		break;
	case Protocol::MoveDir::UP_LEFT:
		sprite->Play(L"PLAYER_DAMAGED_UP_LEFT");
		break;
	}
}

void AIDamaged::OnEnter()
{
	_remainTime = _damagedStateRemainTime;

	auto fsm = GetOwner();
	ASSERT(fsm);

	auto owner = fsm->GetOwner();
	ASSERT(owner);

	auto stat = owner->GetComponent<StatComponent>();
	ASSERT(stat);

	auto gui = owner->GetComponent<GUIComponent>();
	ASSERT(gui);

	{
		//	TEMP;;

		auto healthBar = static_cast<HealthBar*>(gui->FindGuiFromName(L"HealthBar"));
		ASSERT(healthBar);
		healthBar->SetHealthBarRatio();

		auto hit = GActorManager->AddEffect<Hit>();

		auto hitTransform = hit->GetComponent<TransformComponent>();
		ASSERT(hitTransform);

		auto actorTransfrom = owner->GetComponent<TransformComponent>();
		ASSERT(actorTransfrom);

		hitTransform->SetPosition(actorTransfrom->GetPosition());
	}

	UpdateAnimation();
}

void AIDamaged::OnExit()
{
	auto fsm = GetOwner();
	ASSERT(fsm);
	if (fsm->GetCurrentStateName().compare(L"Damaged") != 0)
		return;

	auto owner = fsm->GetOwner();
	ASSERT(owner);

	auto sprite = owner->GetComponent<AnimSpriteComponent>();
	ASSERT(sprite);

	sprite->SetAnimFPS(12.f);
}
