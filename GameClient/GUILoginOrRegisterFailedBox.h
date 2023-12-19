#pragma once
#include "GUIScreen.h"

class GUILable;
class GUIButton;
class GUILoginOrRegisterFailedBox : public GUIScreen
{
public:
	GUILoginOrRegisterFailedBox();
	virtual ~GUILoginOrRegisterFailedBox();

public:
	virtual void	Initialize() override;
	virtual void	Render(const Vector2Int& cameraOffset) override;
	virtual bool	IsInRange(const Vector2Int& position) override;

public:
	virtual void	OnMouseOver(const Vector2Int& position) override;
	virtual void	OnMouseReleased(const Vector2Int& position) override;

private:
	GUILable*		_failedReasonText = nullptr;
	GUIButton*		_closeBtn = nullptr;
};

