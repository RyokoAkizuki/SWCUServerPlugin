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

#include <string>
#include <map>
#include <vector>
#include <sampgdk/a_players.h>

#include "WeaponShop.h"
#include "GameServer.h"
#include "StringUtility.h"

std::map<std::string, std::vector<WeaponShopItem>> g_weaponlist = 
{
	{ "ͽ��", {
		//{ "Fist", "ȭͷ", 0, 0, 1000 },
		{ "Brass Knuckles", "ָͭ��", 1, 0, 1000 },
		{ "Golf Club", "�߶������", 2, 0, 1000 },
		{ "Nightstick", "����", 3, 0, 1000 },
		{ "Knife", "ذ��", 4, 0, 1000 },
		{ "Baseball Bat", "�����", 5, 0, 1000 },
		{ "Shovel", "����", 6, 0, 1000 },
		{ "Pool Cue", "̨���", 7, 0, 1000 },
		{ "Katana", "��ʿ��", 8, 0, 1000 },
		{ "Chainsaw", "���", 9, 0, 1000 }
	} },

	{ "��ǹ", {
		{ "9mm", "9mm��ǹ", 22, 0, 1000 },
		{ "Silenced 9mm", "9mm������ǹ", 23, 0, 1000 },
		{ "Desert Eagle", "ɳĮ֮ӥ��ǹ", 24, 0, 1000 }
	} },

	{ "����ǹ", {
		{ "Shotgun", "����ǹ", 25, 0, 1000 },
		{ "Sawnoff Shotgun", "�̹�����ǹ", 26, 0, 1000 },
		{ "Combat Shotgun", "ս������ǹ", 27, 0, 1000 }
	} },

	{ "���ǹ", {
		{ "Micro SMG/Uzi", "���ȳ��ǹ (Micro-UZI)", 28, 0, 1000 },
		{ "MP5", "MP5 ���ǹ", 29, 0, 1000 },
		{ "Tec-9", "Tec-9 ΢�ͳ��ǹ", 32, 0, 1000 }
	} },

	{ "ͻ����ǹ", {
		{ "AK-47", "AK-47 ͻ����ǹ", 30, 0, 1000 },
		{ "M4", "M4 ����ǹ", 31, 0, 1000 }
	} },

	{ "�ѻ���ǹ", {
		{ "Country Rifle", "��岽ǹ", 33, 0, 1000 },
		{ "Sniper Rifle", "�ѻ���ǹ", 34, 0, 1000 }
	} },

	{ "��������", {
		{ "RPG", "��������� (RPG)", 35, 0, 1000 },
		{ "HS Rocket", "��׷�ٻ��������", 36, 0, 1000 },
		{ "Flamethrower", "����������", 37, 0, 1000 },
		//{ "Minigun", "�����ڻ�ǹ (Minigun)", 38, 0, 1000 }
	} },
	
	{ "Ͷ������", {
		{ "Grenade", "����", 16, 0, 1000 },
		{ "Tear Gas", "���ᵯ", 17, 0, 1000 },
		{ "Molotov Cocktail", "ȼ�յ�", 18, 0, 1000 },
		{ "Satchel Charge", "ըҩ��", 39, 0, 1000 }
	} },

	{ "װ��", {
		{ "Spraycan", "�����", 41, 0, 1000 },
		{ "Fire Extinguisher", "�����", 42, 0, 1000 },
		{ "Camera", "�����", 43, 0, 1000 },
		//{ "Night Vis Goggles", "ҹ�Ӿ�", 44, 0, 1000 },
		//{ "Thermal Goggles", "�ȳ���ҹ�Ӿ�", 45, 0, 1000 },
		{ "Parachute", "����ɡ", 46, 0, 1000 }
	} },
	
	{ "������Ʒ", {
		{ "Purple Dildo", "��ɫ�ļ�����", 10, 0, 1000 },
		{ "Dildo", "��ɫ���𶯰�(С)", 11, 0, 1000 },
		{ "Vibrator", "��ɫ���𶯰�(��)", 12, 0, 1000 },
		{ "Silver Vibrator", "��ɫ���𶯰�", 13, 0, 1000 },
		{ "Flowers", "����", 14, 0, 1000 },
		{ "Cane", "����", 15, 0, 1000 }
	} }
};

void showWeaponShopCategoryDialog(const std::shared_ptr<Account>& player, const std::string& category)
{
	DialogItemList list;
	auto iter = g_weaponlist.find(category);

	if (iter == g_weaponlist.end())
	{
		std::cout << "[Shop] Warning: Unexpected weapon category.\n";
		return;
	}

	for (auto item : iter->second)
	{
		list.append(STR(item.cnname << " �۸�: " << item.price << " ��ҩ: " << item.ammo),
		[player, item]() {
			player->increaseMoney(-1 * item.price, "buy weapon");
			GivePlayerWeapon(player->getInGameID(), item.id, item.ammo);
		}, std::bind(&showWeaponShopDialog, player));
	}

	GameServer::getInstance().dialogmanager.displayListDialog(player, "��������", list, "����", "����");
}

void showWeaponShopDialog(const std::shared_ptr<Account>& player)
{
	DialogItemList list;

	for (auto type : g_weaponlist)
	{
		std::string category = type.first;
		list.append(category, std::bind(&showWeaponShopCategoryDialog, player, category));
	}

	list.append("����", [player]() {
		SetPlayerArmour(player->getInGameID(), 100.0f);
	});

	GameServer::getInstance().dialogmanager.displayListDialog(player, "��������", list, "ȷ��", "ȡ��");
}
