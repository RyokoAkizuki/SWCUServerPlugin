#pragma once

#include <string>

struct HouseInfo
{
	std::string houseid;
	std::string ownerid;
	std::string name;
	std::string password;
	std::string bindedmapname;
	float ex, ey, ez; // entrance, place a sign
	float rotation; // sign direction
	float tx, ty, tz; // teleport location
	float lx, ly, lz, mx, my, mz; // restrcited area, house interior
	int interior; // interior
	int price;
	bool onsale;
	bool rent;
	int64_t expiredtime;

	HouseInfo() : houseid("000000000000000000000000"), ownerid("000000000000000000000000"),
		interior(0), price(0), onsale(false), rent(false), expiredtime(0) {}
};
