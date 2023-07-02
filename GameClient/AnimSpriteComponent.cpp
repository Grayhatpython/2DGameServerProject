#include "pch.h"
#include "AnimSpriteComponent.h"
#include "Actor.h"
#include "Texture.h"
#include "RenderManager.h"
#include "AnimationManager.h"
#include "TransformComponent.h"

COMPONENT_TYPE_DEFINITION(SpriteComponent, AnimSpriteComponent);

AnimSpriteComponent::AnimSpriteComponent(int32 drawOrder)
	: SpriteComponent(drawOrder)
{

}

//	Only One Line Anim Texture
void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	auto owner = GetOwner();

	if (owner->GetState() == ActorState::Active)
	{
		_currentFrame += _animFPS * deltaTime;

		if (_currentFrame >= _currentAnimInfo->maxFrameCount)
		{
			if (_currentAnimInfo->repeat)
				_currentFrame -= _currentAnimInfo->maxFrameCount;
			else
				_currentFrame = _currentAnimInfo->maxFrameCount - 1.f;
		}
		_currentAnimFrameCount = static_cast<int32>(_currentFrame);
	}
}

void AnimSpriteComponent::Render(const Vector2Int& cameraOffset)
{
	auto owner = GetOwner();

	if (owner && owner->GetState() == ActorState::Active && _currentAnimInfo->texture)
	{
		auto transform = owner->GetComponent<TransformComponent>();
		int32 drawPosX = static_cast<int32>(transform->GetPosition().x - cameraOffset.x);
		int32 drawPosY = static_cast<int32>(transform->GetPosition().y - cameraOffset.y);

		int32 srcrectX = 0;
		int32 srcrectY = 0;

		if (_currentAnimInfo->animDir == AnimSpriteDirection::Row)
		{
			srcrectX = static_cast<int32>(_currentAnimInfo->standardXIndex * _currentAnimInfo->width + _currentAnimFrameCount * _currentAnimInfo->width);
			srcrectY = static_cast<int32>(_currentAnimInfo->standardYIndex * _currentAnimInfo->height);

			if (srcrectX > _currentAnimInfo->texture->GetWidth())
				srcrectY += _currentAnimInfo->height;
		}
		else
		{
			srcrectX = static_cast<int32>(_currentAnimInfo->standardXIndex * _currentAnimInfo->width);
			srcrectY = static_cast<int32>(_currentAnimInfo->standardYIndex * _currentAnimInfo->height + _currentAnimFrameCount * _currentAnimInfo->height);

			if (srcrectY > _currentAnimInfo->texture->GetHeight())
				srcrectX += _currentAnimInfo->width;
		}

		SDL_Rect srcrect;
		srcrect.w = static_cast<int32>(_currentAnimInfo->width);
		srcrect.h = static_cast<int32>(_currentAnimInfo->height);
		srcrect.x = srcrectX;
		srcrect.y = srcrectY;

		SDL_Rect dstrect;
		dstrect.w = static_cast<int32>(_currentAnimInfo->width * transform->GetScale());
		dstrect.h = static_cast<int32>(_currentAnimInfo->height * transform->GetScale());
		dstrect.x = static_cast<int32>(drawPosX - dstrect.w / 2);
		dstrect.y = static_cast<int32>(drawPosY - dstrect.h / 2);

		if (HasIntersection(dstrect) == false)
			return;

		::SDL_RenderCopyEx(
			GRenderManager->GetRenderer(),
			_currentAnimInfo->texture->GetSDLTexture(),
			&srcrect,
			&dstrect,
			-Math::ToDegrees(transform->GetRotation()),
			nullptr,
			_currentAnimInfo->flip);
		
	}
}


void AnimSpriteComponent::Play(const std::wstring& name, bool reset)
{
	auto animInfo = GAnimationManager->FindAnimInfo(name);

	ASSERT(animInfo);

	if(reset)
		Reset();

	if (animInfo)
		_currentAnimInfo = animInfo;
}

void AnimSpriteComponent::Reset()
{
	_currentFrame = 0.f;
	_currentAnimFrameCount = 0;
}

