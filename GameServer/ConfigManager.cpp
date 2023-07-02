#include "pch.h"
#include "ConfigManager.h"
#include "JsonParser.h"

ServerConfig ConfigManager::serverConfig{};

bool ConfigManager::LoadConfig()
{
	rapidjson::Document document;
	JsonParser::ParseFromFile("../Common/Config/config.json", document);
		
	const rapidjson::Value& config = document["config"];
	if (config.IsArray())
	{
		for (rapidjson::SizeType i = 0; i < config.Size(); i++)
		{
			const rapidjson::Value& configData = config[i];
			if (configData.IsObject())
			{
				serverConfig.dataPath = configData["dataPath"].GetString();
				serverConfig.dbConnectionString = JsonParser::GetWString(configData, "dbConnectionString");
			}
		}
	}

	ASSERT(serverConfig.dataPath.empty() == false);
	ASSERT(serverConfig.dbConnectionString.empty() == false);
	return true;
}
