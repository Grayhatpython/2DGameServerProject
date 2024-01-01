#pragma once

class Region;
class RegionProcess;
class RegionManager
{
public:
	RegionManager();
	~RegionManager();

public:
	std::shared_ptr<Region>	Add();
	void					Remove(int32 regionId);
	std::shared_ptr<Region>	FindRegionFromId(int32 regionId);

public:
	RegionProcess*			GetRegionProcess() const { return _regionProcess.get(); }

private:
	const RegionManager& operator = (const RegionManager&) = delete;
	RegionManager(const RegionManager&) = delete;

private:
	std::unordered_map<int32, std::shared_ptr<Region>>	_regions;
	int32												_generateRegionId = 1;
	std::unique_ptr<RegionProcess>						_regionProcess;
};

extern RegionManager* GRegionManager;