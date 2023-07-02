#include "pch.h"
#include "GUIScreen.h"
#include "Texture.h"
#include "AssetManager.h"
#include "RenderManager.h"
#include "AssetManager.h"
#include "GUIManager.h"

int32	GUIScreen::_generateId = 1;

GUIScreen::GUIScreen()
{
	_id = _generateId++;
}

GUIScreen::GUIScreen(const std::wstring& name)
	: _name(name)
{
	_id = _generateId++;
}

GUIScreen::~GUIScreen()
{

}

void GUIScreen::Initialize()
{

}

void GUIScreen::Render(const Vector2Int& cameraOffset)
{
}

void GUIScreen::Update(float deltaTime)
{
}

void GUIScreen::OnMousePressed(const Vector2Int& position)
{

}

void GUIScreen::OnMouseReleased(const Vector2Int& position)
{

}

void GUIScreen::OnMouseHeld(const Vector2Int& position)
{
}

void GUIScreen::OnMouseOver(const Vector2Int& position)
{
}

void GUIScreen::OnMouseWheel(const Vector2Int& position)
{
}

void GUIScreen::OnKeyBoardPressed(SDL_Scancode keyCode)
{

}

void GUIScreen::OnKeyBoardHeld(SDL_Scancode keyCode)
{
}

void GUIScreen::OnKeyBoardReleased(SDL_Scancode keyCode)
{

}

void GUIScreen::OnTextInput(const std::wstring& text)
{
}

bool GUIScreen::IsInRange(const Vector2Int& position)
{
	auto width = static_cast<int32>(_background->GetWidth() * _scale) / 2;
	auto height = static_cast<int32>(_background->GetHeight() * _scale) / 2;

	auto leftTopX = _position.x - width;
	auto leftTopY = _position.y - height;
	auto rightBottomX = _position.x + width;
	auto rightBottomY = _position.y + height;

	return leftTopX < position.x&& position.x < rightBottomX&& leftTopY < position.y&& position.y < rightBottomY;
}

void GUIScreen::DrawTexture(Texture* texture, const Vector2Int& position, const Vector2Int& cameraOffset, float scale)
{
	int32 drawPosX = static_cast<int32>(position.x - cameraOffset.x);
	int32 drawPosY = static_cast<int32>(position.y - cameraOffset.y);

	SDL_Rect dstrect;
	dstrect.w = static_cast<int32>(texture->GetWidth() * scale);
	dstrect.h = static_cast<int32>(texture->GetHeight() * scale);
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

void GUIScreen::DrawTextureFromFirst(Texture* texture, const Vector2Int& position, const Vector2Int& cameraOffset, float scale)
{
	int32 drawPosX = static_cast<int32>(position.x - cameraOffset.x);
	int32 drawPosY = static_cast<int32>(position.y - cameraOffset.y);

	SDL_Rect dstrect;
	dstrect.w = static_cast<int32>(texture->GetWidth() * scale);
	dstrect.h = static_cast<int32>(texture->GetHeight() * scale);
	dstrect.x = drawPosX;
	dstrect.y = drawPosY;

	::SDL_RenderCopyEx(
		GRenderManager->GetRenderer(),
		texture->GetSDLTexture(),
		nullptr,
		&dstrect,
		0,
		nullptr,
		SDL_FLIP_NONE);
}

void GUIScreen::SetState(GUIState state)
{
	_state = state;
	if (state == GUIState::Remove)
	{
		GGUIManager->AddRemoveGuiId(_id);
		return;
	}
}


void GUIScreen::SetRelativeMouseMode(bool isRelative)
{
	if (isRelative)
	{
		::SDL_SetRelativeMouseMode(SDL_TRUE);
		::SDL_GetRelativeMouseState(nullptr, nullptr);
	}
	else
	{
		::SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}
