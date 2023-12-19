#include "pch.h"
#include "GUILoginOrRegisterFailedBox.h"
#include "Texture.h"

#include "GUILable.h"
#include "GUIButton.h"
#include "AssetManager.h"
#include "RenderManager.h"

GUILoginOrRegisterFailedBox::GUILoginOrRegisterFailedBox()
{
	auto screenWidth = GRenderManager->GetScreenWidth();
	auto screenHeight = GRenderManager->GetScreenHeight();
	_position = Vector2Int{ screenWidth / 2, screenHeight / 2 };
}

GUILoginOrRegisterFailedBox::~GUILoginOrRegisterFailedBox()
{
	if (_closeBtn)
	{
		delete _closeBtn;
		_closeBtn = nullptr;
	}

	if (_failedReasonText)
	{
		delete _failedReasonText;
		_failedReasonText = nullptr;
	}
}

void GUILoginOrRegisterFailedBox::Initialize()
{
	_closeBtn = new GUIButton();
	_closeBtn->SetPosition(Vector2Int{ _position.x + 50, _position.y + 75 });
	_closeBtn->Initialize();
	_closeBtn->SetFontSize(8);
	_closeBtn->SetState(GUIState::Active);
	_closeBtn->SetText(L"X");
	_closeBtn->SetScale(0.3f);
	_closeBtn->SetTextPosition(Vector2Int{ _closeBtn->GetPosition().x, _closeBtn->GetPosition().y - 2 });
}

void GUILoginOrRegisterFailedBox::Render(const Vector2Int& cameraOffset)
{
	if (_closeBtn)
		_closeBtn->Render(cameraOffset);

	if (_failedReasonText)
		_failedReasonText->Render(cameraOffset);
}

bool GUILoginOrRegisterFailedBox::IsInRange(const Vector2Int& position)
{
	if (_closeBtn->IsInRange(position))
		return true;

	return false;
}

void GUILoginOrRegisterFailedBox::OnMouseOver(const Vector2Int& position)
{
	if (_closeBtn->IsInRange(position))
		_closeBtn->SetMouseOvered(true);
	else
		_closeBtn->SetMouseOvered(false);
}

void GUILoginOrRegisterFailedBox::OnMouseReleased(const Vector2Int& position)
{
	int a = 3;
}
