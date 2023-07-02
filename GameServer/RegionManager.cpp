#include "pch.h"
#include "RegionManager.h"
#include "Region.h"

RegionManager* GRegionManager = nullptr;
//	동기화가 필요 할까...? 
//	지금 설계상 처음에 서버 생성 시 말고는 맵이 생성되고 지워질 일은 없는데..

RegionManager::RegionManager()
{

}

RegionManager::~RegionManager()
{
	std::wcout << L"~RegionManager" << std::endl;
	
	for (auto& region : _regions)
		region.second->Clear();
}

std::shared_ptr<Region> RegionManager::Add()
{
	//	TEMP : 삭제될 일이 거의 없다 -> 풀 필요없을 듯
	//	레퍼런스 관리 해야 할 거 같아서 shared_ptr로 만듬
	auto region = std::make_shared<Region>();
	_regions.insert(std::make_pair(_generateRegionId, region));
	region->SetRegionId(_generateRegionId);
	_generateRegionId++;

	//	TODO
	region->OnlyPushJobAndNotDistribute(&Region::Initialize, 20);

	return region;
}

void RegionManager::Remove(int32 regionId)
{
	_regions.erase(regionId);
}

std::shared_ptr<Region> RegionManager::FindRegionFromId(int32 regionId)
{
	auto iter = _regions.find(regionId);
	if (iter != _regions.end())
		return iter->second;

	return std::shared_ptr<Region>();
}
