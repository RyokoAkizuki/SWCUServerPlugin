#pragma once

#include "Account.h"

void doTeleportToPosForOnePlayerOnFoot(int playerid, float x, float y, float z, int interior = 0, int virtualworld = 0);
void teleportToPos(int playerid, float x, float y, float z, int interior = 0, int virtualworld = 0);
void teleportToPlayer(int playerid, int targetplayerid);
// void withdrawPlayer(const std::shared_ptr<Account>& player);
