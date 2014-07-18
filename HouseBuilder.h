#pragma once

#include "HouseInfo.h"

class HouseBuilder
{
protected:
	bool entrance, name, areamin, areamax;
	HouseInfo info;

public:
	HouseBuilder(float x, float y, float z);

	void setEntrance(float x, float y, float z, float rotation);
	void setName(const std::string& name);
	void setAreaMin(float x, float y, float z);
	void setAreaMax(float x, float y, float z);

	bool finished();
	bool save();
};
