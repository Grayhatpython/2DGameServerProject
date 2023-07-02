#include "pch.h"
#include "GUIImage.h"
#include "Texture.h"
#include "RenderManager.h"

GUIImage::GUIImage()
{
}

GUIImage::~GUIImage()
{
}

void GUIImage::Initialize()
{
}

void GUIImage::Render(const Vector2Int& cameraOffset)
{
	DrawTexture(_background, _position);
}

void GUIImage::DrawTexture(Texture* texture, const Vector2Int& position, const Vector2Int& cameraOffset, float scale)
{
	auto screenWidth = GRenderManager->GetScreenWidth();
	auto screenHeight = GRenderManager->GetScreenHeight();

	int32 drawPosX = static_cast<int32>(position.x - cameraOffset.x);
	int32 drawPosY = static_cast<int32>(position.y - cameraOffset.y);

	int32 width = texture->GetWidth();
	int32 height = texture->GetHeight();
	if (width > screenWidth)
		width = screenWidth;
	if (height > screenHeight)
		height = screenHeight;

	SDL_Rect dstrect;

	dstrect.w = static_cast<int32>(width * scale);
	dstrect.h = static_cast<int32>(height * scale);
	dstrect.x = drawPosX - dstrect.w / 2;
	dstrect.y = drawPosY - dstrect.h / 2;

	::SDL_RenderCopyEx(
		GRenderManager->GetRenderer(),
		texture->GetSDLTexture(),
		nullptr,
		&dstrect,
		0,
		nullptr,
		SDL_FLIP_NONE);
}

bool GUIImage::IsInRange(const Vector2Int& position)
{
	return false;
}

