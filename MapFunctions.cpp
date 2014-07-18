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
