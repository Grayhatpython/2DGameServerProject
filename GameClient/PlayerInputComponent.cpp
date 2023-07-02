#include "pch.h"
#include "PlayerInputComponent.h"
#include "InputManager.h"

COMPONENT_TYPE_DEFINITION(Component, PlayerInputComponent);

PlayerInputComponent::PlayerInputComponent()
{

}

PlayerInputComponent::~PlayerInputComponent()
{

}


void PlayerInputComponent::ProcessInput(const InputState& state)
{	
	_isMoveUpLeftKeyPressed		= state.keyboard.GetKeyValue(SDL_Scancode(SDL_SCANCODE_Q));
	_isMoveUpRightKeyPressed	= state.keyboard.GetKeyValue(SDL_Scancode(SDL_SCANCODE_E));
	_isMoveDownLeftKeyPressed	= state.keyboard.GetKeyValue(SDL_Scancode(SDL_SCANCODE_Z));
	_isMoveDownRightKeyPressed	= state.keyboard.GetKeyValue(SDL_Scancode(SDL_SCANCODE_C));
	
	_isMoveUpKeyPressed			= state.keyboard.GetKeyValue(SDL_Scancode(SDL_SCANCODE_W));
	_isMoveDownKeyPressed		= state.keyboard.GetKeyValue(SDL_Scancode(SDL_SCANCODE_X));
	_isMoveLeftKeyPressed		= state.keyboard.GetKeyValue(SDL_Scancode(SDL_SCANCODE_A));
	_isMoveRightKeyPressed		= state.keyboard.GetKeyValue(SDL_Scancode(SDL_SCANCODE_D));

	_isAutoAttackSkillKeyPressed	= state.keyboard.GetKeyValue(SDL_Scancode(SDL_SCANCODE_SPACE));
	_isBallAttackSkillKeyPressed	= state.keyboard.GetKeyValue(SDL_Scancode(SDL_SCANCODE_B));
}
