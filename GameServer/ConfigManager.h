#pragma once

struct ServerConfig
{
	std::string		dataPath;
	std::wstring	dbConnectionString;
};

class ConfigManager
{
public:
	static bool	LoadConfig();

public:
	static ServerConfig serverConfig;
};

