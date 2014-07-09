#include <iostream>
#include <sstream>
#include <algorithm>

#include "MapCodeParser.h"

void parseMapCode(std::string& source, MapInfo& dest)
{
	ObjectInfo tmpobj;
	VehicleInfo tmpveh;

	std::replace(source.begin(), source.end(), '(', ' ');
	std::replace(source.begin(), source.end(), ')', ' ');
	std::replace(source.begin(), source.end(), ',', ' ');
	std::replace(source.begin(), source.end(), ';', ' ');

	std::stringstream stream(source);

	std::string span;

	while (stream >> span)
	{
		if (span == "CreateObject" || span == "CreateDynamicObject")
		{
			stream >> tmpobj.model >> tmpobj.x >> tmpobj.y >> tmpobj.z >> tmpobj.rx >> tmpobj.ry >> tmpobj.rz;
			dest.objects.push_back(tmpobj);
		}
		else if (span == "CreateVehicle" || span == "AddStaticVehicle" || span == "AddStaticVehicleEx")
		{
			stream >> tmpveh.model >> tmpveh.x >> tmpveh.y >> tmpveh.z >> tmpveh.angle >> tmpveh.color1 >> tmpveh.color2;
			dest.vehicles.push_back(tmpveh);
		}
	}

	std::cout << "[MapParser] Parsed " << dest.objects.size() << " object(s) and " << dest.vehicles.size() << " vehicle(s).\n";
}
