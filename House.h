#pragma once

#include "HouseInfo.h"

class House
{
protected:
	HouseInfo info;
	int areaid;
	int zoneid;
	int signid;
	int entrancezoneid;

public:
	House(HouseInfo& info);
	virtual ~House();

	void init();
	bool setEntrance(float x, float y, float z, float rotation);
	bool setTeleportPos(float x, float y, float z);
	bool setName(const std::string& name);
	bool setPassword(const std::string& password);
	std::string getName();
	bool auth(const std::string& password);
	HouseInfo& _getInfo();
};
