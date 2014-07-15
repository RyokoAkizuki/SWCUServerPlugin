#pragma once

#include "MapInfo.h"

int createObject(const ObjectInfo& obj);
int destroyObject(int id);
int createVehicle(const VehicleInfo& veh);
int destroyVehicle(int id);
int updatePlayerObject(int playerid, float x, float y, float z);
