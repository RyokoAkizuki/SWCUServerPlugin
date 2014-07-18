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
