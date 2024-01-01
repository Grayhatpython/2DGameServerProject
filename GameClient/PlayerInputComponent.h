#pragma once
#include "Component.h"

class PlayerInputComponent : public Component
{
	COMPONENT_TYPE_DECLARATION(PlayerInputComponent);

public:
	PlayerInputComponent();
	virtual ~PlayerInputComponent();

public:
	virtual void	ProcessInput(const struct InputState& state);

public:
	bool		GetIsMoveKeyPressed() const 
	{
		return /*etIsMoveUpLeftKeyPressed() || GetIsMoveUpRightKeyPressed() || GetIsMoveDownLeftKeyPressed() || GetIsMoveDownRightKeyPressed() ||*/
			GetIsMoveUpKeyPressed() || GetIsMoveDownKeyPressed() || GetIsMoveLeftKeyPressed() || GetIsMoveRightKeyPressed();
	}
	bool		GetIsSkillKeyPressed() const
	{
		return GetIsAutoAttackSkillKeyPressed() || GetIsBallAttackSkillKeyPressed();
	}

	bool		GetIsMoveUpLeftKeyPressed() const { return _isMoveUpLeftKeyPressed; }
	bool		GetIsMoveUpRightKeyPressed() const { return _isMoveUpRightKeyPressed; }
	bool		GetIsMoveDownLeftKeyPressed() const { return _isMoveDownLeftKeyPressed; }
	bool		GetIsMoveDownRightKeyPressed() const { return _isMoveDownRightKeyPressed; }
	bool		GetIsMoveUpKeyPressed() const { return _isMoveUpKeyPressed; }
	bool		GetIsMoveDownKeyPressed() const { return _isMoveDownKeyPressed; }
	bool		GetIsMoveLeftKeyPressed() const { return _isMoveLeftKeyPressed; }
	bool		GetIsMoveRightKeyPressed() const { return _isMoveRightKeyPressed; }
	bool		GetIsAutoAttackSkillKeyPressed() const { return _isAutoAttackSkillKeyPressed; }
	bool		GetIsBallAttackSkillKeyPressed() const { return _isBallAttackSkillKeyPressed; }

private:
	bool		_isMoveUpLeftKeyPressed = false;
	bool		_isMoveUpRightKeyPressed = false;
	bool		_isMoveDownLeftKeyPressed = false;
	bool		_isMoveDownRightKeyPressed = false;
	bool		_isMoveUpKeyPressed = false;
	bool		_isMoveDownKeyPressed = false;
	bool		_isMoveLeftKeyPressed = false;
	bool		_isMoveRightKeyPressed = false;
	bool		_isMoveKeyPressed = false;
	bool		_isAutoAttackSkillKeyPressed = false;
	bool		_isBallAttackSkillKeyPressed = false;
};




