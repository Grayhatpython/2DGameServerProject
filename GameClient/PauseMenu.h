#pragma once
#include "GUIScreen.h"

class PauseMenu : public GUIScreen
{
public:
	PauseMenu();
	virtual ~PauseMenu();

public:
	virtual void Initialize() override;
	virtual void ProcessInput(const struct InputState& state) override;
};

