#include "pch.h"
#include "HealthBar.h"
#include "Font.h"
#include "Texture.h"
#include "RenderManager.h"
#include "AssetManager.h"
#include "Creature.h"
#include "StatComponent.h"
#include "TransformComponent.h"

HealthBar::HealthBar()
	:	GUIScreen(L"HealthBar")
{

}

HealthBar::~HealthBar()
{

}

void HealthBar::Initialize()
{
	_healthTexture = GAssetManager->GetTexture("../Assets/GUI/HealthBar.png");
	_background = GAssetManager->GetTexture("../Assets/GUI/HealthBar_BG.png");
}

void HealthBar::Render(const Vector2Int& cameraOffset)
{
	if (GetOwner())
	{
		if (_background)
			GUIScreen::DrawTexture(_background, _position, cameraOffset);

		if (_healthTexture)
			DrawTexture(_healthTexture, _position, cameraOffset);
	}
}

void HealthBar::Update(float deltaTime)
{
	UpdatePosition();
}

void HealthBar::OnMousePressed(const Vector2Int& position)
{

}

void HealthBar::OnMouseReleased(const Vector2Int& position)
{

}


void HealthBar::SetHealthBarRatio()
{
	auto owner = GetOwner();
	if (owner)
	{
		auto stat = owner->GetComponent<StatComponent>();
		ASSERT(stat);

		auto maxHp = stat->GetMaxHp();
		auto hp = static_cast<float>(stat->GetHp());

		_ratio = Math::Clamp(hp / maxHp, 0.f, 1.f);
	}
}

void HealthBar::SetOwner(std::weak_ptr<Creature> owner)
{
	_owner = owner;

	UpdatePosition();
	SetHealthBarRatio();
}

void HealthBar::DrawTexture(Texture* texture, const Vector2Int& position, const Vector2Int& cameraOffset, float scale)
{
	int32 drawPosX = static_cast<int32>(position.x - cameraOffset.x);
	int32 drawPosY = static_cast<int32>(position.y - cameraOffset.y);

	int32 width = static_cast<int32>(texture->GetWidth() * scale);
	float calcWidthFromRatio = width * _ratio;

	SDL_Rect dstrect;
	dstrect.w = static_cast<int32>(calcWidthFromRatio);
	dstrect.h = static_cast<int32>(texture->GetHeight() * scale);
	dstrect.x = drawPosX - width / 2;
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

void HealthBar::UpdatePosition()
{
	auto owner = GetOwner();
	if (owner)
	{
		auto transform = owner->GetComponent<TransformComponent>();
		ASSERT(transform);

		auto ownerPosition = Vector2ToVector2Int(transform->GetPosition());
		_position = ownerPosition + _healthTextureOffset;
	}
}
