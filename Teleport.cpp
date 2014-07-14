#include <sampgdk/a_players.h>
#include <sampgdk/a_vehicles.h>
#include <cstdlib>
#include <ctime>

#include "Teleport.h"

void doTeleportToPosForOnePlayerOnFoot(int playerid, float x, float y, float z, int interior, int virtualworld)
{
	SetPlayerPos(playerid, x, y, z);
	SetPlayerInterior(playerid, interior);
	SetPlayerVirtualWorld(playerid, virtualworld);
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
