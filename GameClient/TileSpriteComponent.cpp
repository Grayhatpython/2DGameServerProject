#include "pch.h"
#include "TileSpriteComponent.h"
#include "Actor.h"
#include "Texture.h"
#include "RenderManager.h"
#include "TransformComponent.h"

COMPONENT_TYPE_DEFINITION(SpriteComponent, TileSpriteComponent);

TileSpriteComponent::TileSpriteComponent(int32 drawOrder)
	:SpriteComponent(drawOrder)
{

}

TileSpriteComponent::~TileSpriteComponent()
{

}

void TileSpriteComponent::Render(const Vector2Int& cameraOffset)
{
	auto owner = GetOwner();
	if (_texture && owner)
	{
		auto transform = owner->GetComponent<TransformComponent>();
		int32 drawPosX = static_cast<int32>(transform->GetPosition().x - cameraOffset.x);
		int32 drawPosY = static_cast<int32>(transform->GetPosition().y - cameraOffset.y);

		for (auto& tileSpriteInfo : _tileSpriteInfos)
		{
			SDL_Rect srcrect;
			srcrect.w = static_cast<int32>(_width);
			srcrect.h = static_cast<int32>(_height);
			srcrect.x = static_cast<int32>(tileSpriteInfo.offsetX * srcrect.w);
			srcrect.y = static_cast<int32>(tileSpriteInfo.offsetY * srcrect.h);

			SDL_Rect dstrect;
			dstrect.w = static_cast<int32>(_width * transform->GetScale());
			dstrect.h = static_cast<int32>(_height * transform->GetScale());
			dstrect.x = static_cast<int32>(drawPosX - dstrect.w / 2);
			dstrect.y = static_cast<int32>(drawPosY - dstrect.h / 2);

			if (HasIntersection(dstrect) == false)
				return;

			::SDL_RenderCopyEx(
				GRenderManager->GetRenderer(),
				_texture->GetSDLTexture(),
				&srcrect,
				&dstrect,
				-Math::ToDegrees(transform->GetRotation()),
				nullptr,
				SDL_FLIP_NONE);
		}
	}
}
