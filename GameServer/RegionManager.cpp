#include "pch.h"
#include "RegionManager.h"
#include "Region.h"

RegionManager* GRegionManager = nullptr;
//	����ȭ�� �ʿ� �ұ�...? 
//	���� ����� ó���� ���� ���� �� ����� ���� �����ǰ� ������ ���� ���µ�..

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
	//	TEMP : ������ ���� ���� ���� -> Ǯ �ʿ���� ��
	//	���۷��� ���� �ؾ� �� �� ���Ƽ� shared_ptr�� ����
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
