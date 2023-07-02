#include "pch.h"
#include "PauseMenu.h"
#include "PlayGameScene.h"
#include "InputManager.h"
#include "GameSceneManager.h"
PauseMenu::PauseMenu()
{

}

PauseMenu::~PauseMenu()
{
	SetRelativeMouseMode(true);
	auto currentGameScene = GGameSceneManager->GetGameScene();
	if (currentGameScene->GetGameSceneType() == GameSceneType::Play)
	{
		auto playGameScene = std::static_pointer_cast<PlayGameScene>(currentGameScene);
		playGameScene->SetState(GameState::Play);
	}
}

void PauseMenu::Initialize()
{
	GUIScreen::Initialize();

	GGameSceneManager->GetGameScene()->SetState(GameState::Paused);
	SetTitlePosition(Vector2{ 300.f, 300.f });
	SetNextButtonPosition(Vector2{ 300.f, 200.f });
	SetbackgroundPosition(Vector2{ 300.f, 250.f });
	SetRelativeMouseMode(false);
	AddButton("ResumeButton", [p = shared_from_this()]()
		{
			p->Close();
		});
	SetTitle("PauseTitle");

}

void PauseMenu::ProcessInput(const InputState& state)
{
	GUIScreen::ProcessInput(state);

	if (state.keyboard.GetKeyState(SDL_SCANCODE_ESCAPE) == ButtonState::Released)
		Close();
}
