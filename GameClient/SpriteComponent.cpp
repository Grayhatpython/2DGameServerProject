#include "pch.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "Actor.h"
#include "RenderManager.h"
#include "Texture.h"

COMPONENT_TYPE_DEFINITION(Component, SpriteComponent);

SpriteComponent::SpriteComponent(int32 drawOrder)
	: _drawOrder(drawOrder)
{

}

SpriteComponent::~SpriteComponent()
{

}

void SpriteComponent::Render(const Vector2Int& cameraOffset)
{
	auto owner = GetOwner();
	if (_texture && owner)
	{
		auto transform = owner->GetComponent<TransformComponent>();
		int32 drawPosX = static_cast<int32>(transform->GetPosition().x - cameraOffset.x);
		int32 drawPosY = static_cast<int32>(transform->GetPosition().y - cameraOffset.y);

		SDL_Rect dstrect;
		dstrect.w = static_cast<int32>(_textureWidth * transform->GetScale());
		dstrect.h = static_cast<int32>(_textureHeight * transform->GetScale());
		dstrect.x = static_cast<int32>(drawPosX - dstrect.w / 2);
		dstrect.y = static_cast<int32>(drawPosY - dstrect.h / 2);

		if (HasIntersection(dstrect) == false)
			return;

		::SDL_RenderCopyEx(
			GRenderManager->GetRenderer(),
			_texture->GetSDLTexture(),
			nullptr,
			&dstrect,
			-Math::ToDegrees(transform->GetRotation()),
			nullptr,
			SDL_FLIP_NONE);
	}
}

void SpriteComponent::SetTexture(Texture* texture)
{
	_texture = texture;

	_textureWidth = texture->GetWidth();
	_textureHeight = texture->GetHeight();
}

bool SpriteComponent::HasIntersection(const SDL_Rect& rect)
{
	SDL_Rect screen;
	screen.w = GRenderManager->GetScreenWidth();
	screen.h = GRenderManager->GetScreenHeight();
	screen.x = 0;
	screen.y = 0;
	if (::SDL_HasIntersection(&screen, &rect))
		return true;

	return false;
}
