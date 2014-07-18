#pragma once

#include <string>
#include <cstdint>
#include <memory>

#include "HouseBuilder.h"

struct AccountInfo
{
	// From Database
	std::string userid;
	std::string logname;
	std::string nickname;
	int money;
	int adminlevel;
	bool disabled;
	bool registered;

	// In-game
	int ingameid;
	bool loggedin;
	uint64_t session;

	bool freezed;
	bool muted;

	int64_t logtime;

	std::shared_ptr<HouseBuilder> housebuilder;

	// last frame data
	float x, y, z;

	AccountInfo() :
		money(0), adminlevel(0), disabled(false), loggedin(false), registered(false), ingameid(-1), session(0),
		freezed(false), muted(false)
	{}
};
