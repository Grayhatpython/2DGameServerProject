#pragma once
#include "GUIScreen.h"

class GUILable;
class GUIButton;
class GUIServerSelectionForm : public GUIScreen
{
public:
	GUIServerSelectionForm();
	virtual ~GUIServerSelectionForm();

public:
	virtual void			Initialize() override;
	virtual void			Render(const Vector2Int& cameraOffset) override;

public:
	virtual void			OnMouseReleased(const Vector2Int& position) override;
	virtual void			OnMouseOver(const Vector2Int& position) override;

public:
	void					SetServerInfos(const std::vector<Protocol::ServerInfo> serverInfos);

private:
	void					OnClickGameStartButton();

private:
	GUILable*							_titleText = nullptr;

	std::vector<Protocol::ServerInfo>	_serverInfos;
	std::vector<GUILable*>				_serverNames;
	std::vector<GUIButton*>				_serverNamesBtn;

	GUIButton*							_startBtn = nullptr;

	int32								_selectServerIndex = -1;
};

