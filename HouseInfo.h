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
