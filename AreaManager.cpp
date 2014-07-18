/*
 * Copyright 2014 Yukino Hayakawa<tennencoll@gmail.com>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
