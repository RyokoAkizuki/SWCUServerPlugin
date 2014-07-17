#include <sampgdk/a_vehicles.h>
#include <sampgdk/streamer.h>

#include "MapFunctions.h"

int createObject(const ObjectInfo& obj)
{
	return CreateDynamicObject(obj.model, obj.x, obj.y, obj.z, obj.rx, obj.ry, obj.rz, -1, -1, -1, 500.0f, 0.0f);
}

int destroyObject(int id)
{
	return DestroyDynamicObject(id);
}

int createVehicle(const VehicleInfo& veh)
{
	return CreateVehicle(veh.model, veh.x, veh.y, veh.z, veh.angle, veh.color1, veh.color2, 120);
}

int destroyVehicle(int id)
{
	return DestroyVehicle(id);
}

int updatePlayerObject(int playerid, float x, float y, float z)
{
	return Streamer_UpdateEx(playerid, x, y, z, -1, -1);
}
