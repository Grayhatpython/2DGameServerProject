#pragma once
#include "GUIScreen.h"

class Font;
class Texture;
class Creature;
class HealthBar : public GUIScreen
{
public:
	HealthBar();
	virtual ~HealthBar();

public:
	virtual void	Initialize() override;
	virtual void	Render(const Vector2Int& cameraOffset) override;
	virtual void	Update(float deltaTime) override;

public:
	virtual void	OnMousePressed(const Vector2Int& position) override;
	virtual void	OnMouseReleased(const Vector2Int& position) override;

public:
	void			SetHealthBarRatio();

	void						SetOwner(std::weak_ptr<Creature> owner);
	std::shared_ptr<Creature>	GetOwner() { return _owner.lock(); }

protected:
	virtual void	DrawTexture(Texture* texture, const Vector2Int& position, const Vector2Int& cameraOffset = Vector2Int{0,0}, float scale = 1.f) override;

private:
	void UpdatePosition();

private:
	//Font*			_font = nullptr;
	Texture*					_healthTexture = nullptr;
	Vector2Int					_healthTextureOffset{0, 25};
	float						_ratio = 1.f;
	std::weak_ptr<Creature>		_owner;
};

