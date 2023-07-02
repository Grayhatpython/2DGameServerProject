#pragma once

class Font;
class Texture;
class AssetManager
{
public:
	AssetManager();
	~AssetManager();

public:
	void Clear();

public:
	void LoadText(const std::string& fileName);
	const std::string& GetText(const std::string& textKey);

public:
	Font*		GetFont(const std::string& fileName);
	Texture*	GetTexture(const std::string& fileName);

private:
	const AssetManager& operator = (const AssetManager&) = delete;
	AssetManager(const AssetManager&) = delete;

private:
	//	Font
	std::unordered_map<std::string, Font*>			_fonts;
	//	Texture
	std::unordered_map<std::string, Texture*>		_textures;
	//	Text
	std::unordered_map<std::string, std::string>	_texts;

};

extern AssetManager* GAssetManager;
