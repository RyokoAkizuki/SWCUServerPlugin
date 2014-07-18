#pragma once

#include "HouseInfo.h"

class HouseBuilder
{
protected:
	HouseInfo info;
	bool updating;

public:
	HouseBuilder();

	void setName(const std::string& name);
	void setEntrance(float x, float y, float z, float rotation);
	void setOwner(const std::string& playerid);
	void startUpdateArea();
	void stopUpdateArea();
	bool isUpdatingArea();
	void updateArea(float x, float y, float z);
	void save();
};
