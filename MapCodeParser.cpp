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

#include <iostream>
#include <sstream>
#include <algorithm>

#include "MapCodeParser.h"

void parseMapCode(std::string& source, MapInfo& dest)
{
	ObjectInfo tmpobj;
	VehicleInfo tmpveh;

	std::replace(source.begin(), source.end(), '(', ' ');
	std::replace(source.begin(), source.end(), ')', ' ');
	std::replace(source.begin(), source.end(), ',', ' ');
	std::replace(source.begin(), source.end(), ';', ' ');

	std::stringstream stream(source);

	std::string span;

	while (stream >> span)
	{
		if (span == "CreateObject" || span == "CreateDynamicObject")
		{
			stream >> tmpobj.model >> tmpobj.x >> tmpobj.y >> tmpobj.z >> tmpobj.rx >> tmpobj.ry >> tmpobj.rz;
			dest.objects.push_back(tmpobj);
		}
		else if (span == "CreateVehicle" || span == "AddStaticVehicle" || span == "AddStaticVehicleEx")
		{
			stream >> tmpveh.model >> tmpveh.x >> tmpveh.y >> tmpveh.z >> tmpveh.angle >> tmpveh.color1 >> tmpveh.color2;
			dest.vehicles.push_back(tmpveh);
		}
	}

	std::cout << "[MapParser] Parsed " << dest.objects.size() << " object(s) and " << dest.vehicles.size() << " vehicle(s).\n";
}
