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

#include <sampgdk/a_players.h>
#include <sampgdk/a_vehicles.h>
#include <cstdlib>
#include <ctime>

#include "Teleport.h"
#include "MapFunctions.h"

void doTeleportToPosForOnePlayerOnFoot(int playerid, float x, float y, float z, int interior, int virtualworld)
{
	SetPlayerPos(playerid, x, y, z);
	SetPlayerInterior(playerid, interior);
	SetPlayerVirtualWorld(playerid, virtualworld);
	updatePlayerObject(playerid, x, y, z);
}

void teleportToPos(int playerid, float x, float y, float z, int interior, int virtualworld)
{
	doTeleportToPosForOnePlayerOnFoot(playerid, x, y, z, interior, virtualworld);
	if (IsPlayerInAnyVehicle(playerid) && GetPlayerVehicleSeat(playerid) == 0 /* driver */)
	{
		int vid = GetPlayerVehicleID(playerid);
		SetVehicleVirtualWorld(vid, virtualworld);
		LinkVehicleToInterior(vid, interior);
		SetVehiclePos(vid, x, y, z);
		// NOTICE: Here uses a BIG loop.
		int max = GetMaxPlayers();
		for (int i = 0; i < max; ++i)
		{
			if (GetPlayerVehicleID(i) == vid)
			{
				doTeleportToPosForOnePlayerOnFoot(i, x, y, z, interior, virtualworld);
				PutPlayerInVehicle(i, vid, GetPlayerVehicleSeat(i));
			}
		}
	}
}

void teleportToPlayer(int playerid, int targetplayerid)
{
	float pos[3];
	GetPlayerPos(targetplayerid, &pos[0], &pos[1], &pos[2]);
	srand(time(NULL));
	pos[0] += ((float)((float)rand() - ((float)RAND_MAX / 2)) / (float)RAND_MAX) * 5;
	pos[1] += ((float)((float)rand() - ((float)RAND_MAX / 2)) / (float)RAND_MAX) * 5;
	pos[2] += ((float)rand() / (float)RAND_MAX) * 3;
	int it = GetPlayerInterior(targetplayerid);
	int vw = GetPlayerVirtualWorld(targetplayerid);
	teleportToPos(playerid, pos[0], pos[1], pos[2], it, vw);
}
/*
void withdrawPlayer(const std::shared_ptr<Account>& player)
{
	float pos[3];
	GetPlayerPos(player->getInGameID(), &pos[0], &pos[1], &pos[2]);
	float d[3];
	d[0] = pos[0] - player->_getAccountInfo().x;
	d[1] = pos[1] - player->_getAccountInfo().y;
	d[2] = pos[2] - player->_getAccountInfo().z;
	float k = sqrt((20 * 20) / (d[0] * d[0] + d[1] * d[1] + d[2] * d[2]));
	teleportToPos(player->getInGameID(), pos[0] - (k * d[0]), pos[1] - (k * d[1]), pos[2],
		GetPlayerInterior(player->getInGameID()), GetPlayerVirtualWorld(player->getInGameID()));
}
*/
