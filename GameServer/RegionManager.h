#pragma once

class Region;
class RegionManager
{
public:
	RegionManager();
	~RegionManager();

public:
	std::shared_ptr<Region>	Add();
	void					Remove(int32 regionId);
	std::shared_ptr<Region>	FindRegionFromId(int32 regionId);

private:
	const RegionManager& operator = (const RegionManager&) = delete;
	RegionManager(const RegionManager&) = delete;

private:
	std::unordered_map<int32, std::shared_ptr<Region>>	_regions;
	int32												_generateRegionId = 1;
};

extern RegionManager* GRegionManager;