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
	bool jailed;

	int64_t logtime;

	std::shared_ptr<HouseBuilder> housebuilder;

	// last frame data
	float x, y, z;

	AccountInfo() :
		money(0), adminlevel(0), disabled(false), loggedin(false), registered(false), ingameid(-1), session(0),
		freezed(false), muted(false), jailed(false)
	{}
};
