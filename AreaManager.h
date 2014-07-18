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

#pragma once

#include <map>
#include <functional>
#include <vector>

#include "Account.h"

typedef std::function<void(const std::shared_ptr<Account>&)> AreaCallback;

class AreaManager
{
protected:
	// <id, <enter, exit>>
	std::map<int, std::pair<AreaCallback, AreaCallback>> areas;

public:
	void registerArea(int id, const AreaCallback& cbenter, const AreaCallback& cbexit = [](const std::shared_ptr<Account>&){});
	void destroyArea(int id);

	void _callbackEnter(const std::shared_ptr<Account>& player, int areaid);
	void _callbackExit(const std::shared_ptr<Account>& player, int areaid);
};
