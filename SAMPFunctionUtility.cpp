#include <sampgdk/a_players.h>
#include <sampgdk/a_samp.h>

#include "SAMPFunctionUtility.h"

std::string getPlayerNameFixed(int playerid)
{
	std::string realname;
	char temp[24 * 4];
	GetPlayerName(playerid, temp, 24 * 4);
	for (int i = 0; (i < 24 * 4 && temp[i] != 0); ++i)
	{
		if (temp[i] != -1)
		{
			realname.push_back(temp[i]);
		}
	}
	return realname;
}

std::string getPlayerIP(int playerid)
{
	char ip[16];
	GetPlayerIp(playerid, ip, 16);
	return ip;
}

std::string getGPCI(int playerid)
{
	char gpcidest[25];
	gpci(playerid, gpcidest, 25);
	return gpcidest;
}
