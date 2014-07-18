#pragma once

#include <string>
#include <cstdint>

struct HouseInfo
{
	std::string houseid;
	std::string ownerid;
	std::string name;
	std::string password;
	std::string bindedmapname;
	float ex, ey, ez; // place a sign
	float rotation; // sign direction
	float tx, ty, tz; // exit teleport location
	float lx, ly, lz, mx, my, mz; // restrcited area, house interior
	int interior; // interior
	int price;
	bool onsale;
	bool rent;
	int64_t expiredtime;

	HouseInfo() : houseid("000000000000000000000000"), ownerid("000000000000000000000000"),
		ex(0.0), ey(0.0), ez(0.0), rotation(0.0), tx(0.0), ty(0.0), tz(0.0),
		lx(0.0), ly(0.0), lz(0.0), mx(0.0), my(0.0), mz(0.0),
		interior(0), price(0), onsale(false), rent(false), expiredtime(0) {}
};
