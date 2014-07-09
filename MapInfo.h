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
