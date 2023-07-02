#include "pch.h"
#include "AssetManager.h"
#include "Font.h"
#include "Texture.h"
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
AssetManager* GAssetManager = nullptr;

AssetManager::AssetManager()
{

}

AssetManager::~AssetManager()
{
	Clear();
}

void AssetManager::Clear()
{
	for (auto font : _fonts)
	{
		delete font.second;
		font.second = nullptr;
	}

	for (auto texture : _textures)
	{
		delete texture.second;
		texture.second = nullptr;
	}

	_textures.clear();
	_fonts.clear();
}

void AssetManager::LoadText(const std::string& fileName)
{
	_texts.clear();
	std::ifstream file(fileName);
	if(file.is_open() == false)
	{
		SDL_Log("Text file %s not found", fileName.c_str());
		return;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();

	std::string contents = fileStream.str();

	rapidjson::StringStream  jsonStr(contents.c_str());
	rapidjson::Document document;
	document.ParseStream(jsonStr);
	if(document.IsObject() == false)
	{
		SDL_Log("Text file %s is not valid JSON", fileName.c_str());
		return;
	}

	const rapidjson::Value& actions = document["TextMap"];
	for (rapidjson::Value::ConstMemberIterator iter = actions.MemberBegin(); iter != actions.MemberEnd(); ++iter)
	{
		if (iter->name.IsString() && iter->value.IsString())
			_texts.insert(std::make_pair(iter->name.GetString(), iter->value.GetString()));
	}
}

const std::string& AssetManager::GetText(const std::string& textKey)
{
	static std::string errorMsg("TextKey not Found");
	auto iter = _texts.find(textKey);
	if (iter != _texts.end())
		return iter->second;
	else
		return errorMsg;
}

Font* AssetManager::GetFont(const std::string& fileName)
{
	Font* font = nullptr;
	auto iter = _fonts.find(fileName);
	if (iter != _fonts.end())
		font = iter->second;
	else
	{
		font = new Font();
		if (font->Load(fileName))
			_fonts.insert(std::make_pair(fileName, font));
		else
			return nullptr;
	}

	return font;
}

Texture* AssetManager::GetTexture(const std::string& fileName)
{
	Texture* texture = nullptr;
	auto iter = _textures.find(fileName);
	if (iter != _textures.end())
		texture = iter->second;
	else
	{
		texture = new Texture();
		if (texture->Load(fileName))
			_textures.insert(std::make_pair(fileName, texture));
		else
			return nullptr;
	}

	return texture;
}
