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

#include <iostream>
#include <vector>

struct ObjectInfo
{
	int model;
	float x, y, z;
	float rx, ry, rz;
};

struct VehicleInfo
{
	int model;
	float x, y, z;
	float angle;
	int color1, color2;
};

struct MapInfo
{
	std::string name;
	std::vector<ObjectInfo> objects;
	std::vector<VehicleInfo> vehicles;
	bool autoload;
};

std::ostream& operator<<(std::ostream& out, const ObjectInfo& obj);
std::ostream& operator<<(std::ostream& out, const VehicleInfo& veh);
std::ostream& operator<<(std::ostream& out, const MapInfo& map);
