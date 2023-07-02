#pragma once

class SpriteComponent;
class RenderManager
{
public:
	RenderManager();
	~RenderManager();

public:
	bool Initialize(int32 screenWidth, int32 screenHeight);
	void SetBackBufferColor(const SDL_Color& color);
	void Present();

public:
	void Clear();
	void Render();

public:

	SDL_Renderer*	GetRenderer() { return _renderer; }
	void			SetCameraOffset(const Vector2Int& offset) { _cameraOffset = offset; }
	
	void			SetScreenWidth(int32 width) { _screenWidth = width; }
	void			SetScreenHeight(int32 height) { _screenHeight = height; }
	int32			GetScreenWidth() { return _screenWidth; }
	int32			GetScreenHeight() { return _screenHeight; }

private:
	const RenderManager& operator = (const RenderManager&) = delete;
	RenderManager(const RenderManager&) = delete;

private:
	
	SDL_Window*		_window = nullptr;
	SDL_Renderer*	_renderer = nullptr;
	SDL_Color		_backBufferColor{};

	//	TODO
	Vector2Int										_cameraOffset{};

	int32											_screenWidth = 0;
	int32											_screenHeight = 0;
};

extern RenderManager* GRenderManager;