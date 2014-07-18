#include <sampgdk/streamer.h>

#include "AreaManager.h"

void AreaManager::registerArea(int id, const AreaCallback& cbenter, const AreaCallback& cbexit)
{
	areas.insert(std::make_pair(id, std::make_pair(cbenter, cbexit)));
}

void AreaManager::destroyArea(int id)
{
	areas.erase(id);
}

void AreaManager::_callbackEnter(const std::shared_ptr<Account>& player, int areaid)
{
	auto iter = areas.find(areaid);
	if (iter == areas.end())
	{
		std::cout << "[AreaManager] callbackEnter: Warning: invalid areaid.\n";
		return;
	}
	iter->second.first(player);
}

void AreaManager::_callbackExit(const std::shared_ptr<Account>& player, int areaid)
{
	auto iter = areas.find(areaid);
	if (iter == areas.end())
	{
		std::cout << "[AreaManager] callbackExit: Warning: invalid areaid.\n";
		return;
	}
	iter->second.second(player);
}
