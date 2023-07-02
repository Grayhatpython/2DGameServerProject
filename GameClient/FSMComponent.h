#include "Component.h"
#include "AIState.h"

class FSMComponent : public Component
{
	COMPONENT_TYPE_DECLARATION(FSMComponent);

public:
	FSMComponent();
	virtual ~FSMComponent();

public:
	virtual void			Update(float deltaTime) override;
	void					ChangeState(const std::wstring& name);
	void					ChangeState(const Protocol::AIState& state);
	void					ChangeState(const Protocol::PositionInfo& info);
	
private:
	const WCHAR*	FindStringFromAIState(const Protocol::AIState& state);

public:
	template<typename T>
	T*		AddState()
	{
		std::unique_ptr<AIState> state = std::make_unique<T>();
		const auto& name = state->GetName();
		_states[name] = std::move(state);
		_states[name]->SetOwner(this);
		return static_cast<T*>(_states[name].get());

	}
	void	RemoveState(const std::wstring& name);

public:
	const std::wstring&		GetCurrentStateName() { return _currentState->GetName(); }
	AIState*				GetCurrentState() { return _currentState; }
	void					SetCurrentState(AIState* state) { _currentState = state; }

private:
	std::unordered_map<std::wstring, std::unique_ptr<AIState>>	_states;
	AIState*													_currentState = nullptr;
};

