#pragma once

enum class GUIState
{
	Active,
	DeActive,
	Remove,
};

class Texture;
class GUIScreen 
{
public:
	GUIScreen();
	GUIScreen(const std::wstring& name);
	virtual ~GUIScreen();

public:
	virtual void			Initialize();
	virtual void			Render(const Vector2Int& cameraOffset);
	virtual void			Update(float deltaTime);

public:
	virtual void			OnMousePressed(const Vector2Int& position);
	virtual void			OnMouseReleased(const Vector2Int& position);
	virtual void			OnMouseHeld(const Vector2Int& position);
	virtual void			OnMouseOver(const Vector2Int& position);
	virtual void			OnMouseWheel(const Vector2Int& position);
	virtual void			OnKeyBoardPressed(SDL_Scancode keyCode);
	virtual void			OnKeyBoardHeld(SDL_Scancode keyCode);
	virtual void			OnKeyBoardReleased(SDL_Scancode keyCode);
	virtual void			OnTextInput(const std::wstring& text);

public:
	virtual bool			IsInRange(const Vector2Int& position);

protected:
	//	From Center
	virtual void			DrawTexture(Texture* texture, const Vector2Int& position, const Vector2Int& cameraOffset = Vector2Int{0, 0});
	//	From First
	virtual void			DrawTextureFromFirst(Texture* texture, const Vector2Int& position, const Vector2Int& cameraOffset = Vector2Int{0, 0});

public:
	void					SetState(GUIState state);
	GUIState				GetState() const { return _state; }

	void					SetPosition(const Vector2Int& position) { _position = position; }
	const Vector2Int&		GetPosition() const { return _position; }
	
	void					SetName(const std::wstring& name) { _name = name; }
	const std::wstring&		GetName() const { return _name; }

	float					GetScale() { return _scale; }
	void					SetScale(float scale) { _scale = scale; }

	Texture*				GetBackgroundTexture() { return _background; }

	int32					GetId() { return _id; }
	void					SetRelativeMouseMode(bool isRelative);

	void					SetBackGround(Texture* background) { _background = background; }

protected:
	GUIState				_state = GUIState::DeActive;

	Vector2Int				_position{};
	Texture*				_background = nullptr;

	std::wstring			_name;
	int32					_id = 1;

	static int32			_generateId;

	float					_scale = 1.f;
};

