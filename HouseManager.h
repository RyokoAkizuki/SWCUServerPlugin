#pragma once

#include <string>
#include <map>
#include <memory>

#include "House.h"

class HouseManager
{
protected:
	std::map<std::string, std::shared_ptr<House>> houses;

public:
	void reloadAll();
	std::shared_ptr<House> findHouse(const std::string& houseid);
	std::shared_ptr<House> addHouse(HouseInfo& info);
	void initAll();
};
