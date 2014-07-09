#include <string>

#include "MapInfo.h"

std::ostream& operator<<(std::ostream& out, const ObjectInfo& obj)
{
	out << "Object(" << obj.model << ", " << obj.x << ", " << obj.y << ", " << obj.z << ", "
		<< obj.rx << ", " << obj.ry << ", " << obj.rz << ")";
	return out;
}

std::ostream& operator<<(std::ostream& out, const VehicleInfo& veh)
{
	out << "Vehicle(" << veh.model << ", " << veh.x << ", " << veh.y << ", " << veh.z << ", "
		<< veh.angle << ", " << veh.color1 << ", " << veh.color2 << ")";
	return out;
}

std::ostream& operator<<(std::ostream& out, const MapInfo& map)
{
	out << "Map(" << map.name << ", objects: " << map.objects.size() << ", vehicles: " << map.vehicles.size() << ", autoload: " << map.autoload << ")";
	return out;
}
