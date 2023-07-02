#include "pch.h"
#include "RenderManager.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Actor.h"
#include "Grid.h"
#include "Tile.h"
#include "ActorManager.h"
#include "GUIManager.h"
#include "TileSpriteComponent.h"
#include "AnimSpriteComponent.h"

RenderManager* GRenderManager = nullptr;

RenderManager::RenderManager()
{

}

RenderManager::~RenderManager()
{
	::SDL_StopTextInput();
	::TTF_Quit();
	::IMG_Quit();
	if (_renderer)
	{
		::SDL_DestroyRenderer(_renderer);
		_renderer = nullptr;
	}
	if (_window)
	{
		::SDL_DestroyWindow(_window);
		_window = nullptr;
	}
	::SDL_Quit();
}

bool RenderManager::Initialize(int32 screenWidth, int32 screenHeight)
{
	if (::SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		::SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	_window = SDL_CreateWindow("GameClient", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	if (!_window)
	{
		::SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!_renderer)
	{
		::SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	if (::IMG_Init(IMG_INIT_PNG) == 0)
	{
		::SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	if (::TTF_Init() != 0)
	{
		::SDL_Log("Failed to initialize SDL_ttf");
		return false;
	}

	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	::SDL_StartTextInput();
	::SDL_RenderClear(_renderer);

	return true;
}

void RenderManager::SetBackBufferColor(const SDL_Color& color)
{
	::SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
	::SDL_RenderClear(_renderer);
}

void RenderManager::Present()
{
	::SDL_RenderPresent(_renderer);
}

void RenderManager::Render()
{
	//Performance p;

	//	Tile
	auto grid = GActorManager->GetGrid();
	auto& tiles = grid->GetTiles();
		
	int32 width = grid->GetTileWidth() * static_cast<int32>(grid->GetTileScale());
	int32 height = grid->GetTileHeight() * static_cast<int32>(grid->GetTileScale());
	int32 startXIndex = _cameraOffset.x / width;
	int32 startYIndex = _cameraOffset.y / height;

	int32 rows =  _screenHeight/ height + 1;
	int32 columns = _screenWidth / width + 1;

	for (int32 row = 0; row < rows; row++)
	{
		for (int32 column = 0; column < columns; column++)
		{
			int32 calYIndex = row + startYIndex;
			int32 calXIndex = column + startXIndex;

			if (calYIndex >= grid->GetRows())
				calYIndex = grid->GetRows() - 1;

			if (calXIndex >= grid->GetColumns())
				calXIndex = grid->GetColumns() - 1;

			auto sprite = tiles[calYIndex][calXIndex]->GetComponent<SpriteComponent>();
			if (sprite)
				sprite->Render(_cameraOffset);
		}
	}

	//	Actor
	std::vector<SpriteComponent*> sprites;
	for (auto& actor : GActorManager->GetActors())
	{
		auto sprite = actor.second->GetComponent<SpriteComponent>();
		if (sprite)
			sprites.push_back(sprite);
	}

	for (auto& effect : GActorManager->GetEffects())
	{
		auto sprite = effect.second->GetComponent<SpriteComponent>();
		if (sprite)
			sprites.push_back(sprite);
	}

	std::sort(sprites.begin(), sprites.end(), [](SpriteComponent* a, SpriteComponent* b) {
		return a->GetDrawOrder() < b->GetDrawOrder();
		});

	for (auto& sprite : sprites)
		sprite->Render(_cameraOffset);

	GGUIManager->Render(_cameraOffset);
}


void RenderManager::Clear()
{
	_cameraOffset = Vector2Int{};
}
