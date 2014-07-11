#include <sstream>
#include <string>
#include <sampgdk/a_players.h>

#include "WeaponShop.h"
#include "GameServer.h"

std::map<std::string, std::vector<WeaponShopItem>> g_weaponlist = 
{
	{ "ͽ��", {
		{ "Fist", "ȭͷ", 0, 100, 100 },
		{ "Brass Knuckles", "ָͭ��", 1, 100, 100 },
		{ "Golf Club", "�߶������", 2, 100, 100 },
		{ "Nightstick", "����", 3, 100, 100 },
		{ "Knife", "ذ��", 4, 100, 100 },
		{ "Baseball Bat", "�����", 5, 100, 100 },
		{ "Shovel", "����", 6, 100, 100 },
		{ "Pool Cue", "̨���", 7, 100, 100 },
		{ "Katana", "��ʿ��", 8, 100, 100 },
		{ "Chainsaw", "���", 9, 100, 100 }
	} },

	{ "��ǹ", {
		{ "9mm", "9mm��ǹ", 22, 100, 100 },
		{ "Silenced 9mm", "9mm������ǹ", 23, 100, 100 },
		{ "Desert Eagle", "ɳĮ֮ӥ��ǹ", 24, 100, 100 }
	} },

	{ "����ǹ", {
		{ "Shotgun", "����ǹ", 25, 100, 100 },
		{ "Sawnoff Shotgun", "�̹�����ǹ", 26, 100, 100 },
		{ "Combat Shotgun", "ս������ǹ", 27, 100, 100 }
	} },

	{ "���ǹ", {
		{ "Micro SMG/Uzi", "���ȳ��ǹ (Micro-UZI)", 28, 100, 100 },
		{ "MP5", "MP5 ���ǹ", 29, 100, 100 },
		{ "Tec-9", "Tec-9 ΢�ͳ��ǹ", 32, 100, 100 }
	} },

	{ "ͻ����ǹ", {
		{ "AK-47", "AK-47 ͻ����ǹ", 30, 100, 100 },
		{ "M4", "M4 ����ǹ", 31, 100, 100 }
	} },

	{ "�ѻ���ǹ", {
		{ "Country Rifle", "��岽ǹ", 33, 100, 100 },
		{ "Sniper Rifle", "�ѻ���ǹ", 34, 100, 100 }
	} },

	{ "��������", {
		{ "RPG", "��������� (RPG)", 35, 100, 100 },
		{ "HS Rocket", "��׷�ٻ��������", 36, 100, 100 },
		{ "Flamethrower", "����������", 37, 100, 100 },
		{ "Minigun", "�����ڻ�ǹ (Minigun)", 38, 100, 100 }
	} },
	
	{ "Ͷ������", {
		{ "Grenade", "����", 16, 100, 100 },
		{ "Tear Gas", "���ᵯ", 17, 100, 100 },
		{ "Molotov Cocktail", "ȼ�յ�", 18, 100, 100 },
		{ "Satchel Charge", "ըҩ��", 39, 100, 100 }
	} },

	{ "װ��", {
		{ "Spraycan", "�����", 41, 100, 100 },
		{ "Fire Extinguisher", "�����", 42, 100, 100 },
		{ "Camera", "�����", 43, 100, 100 },
		{ "Night Vis Goggles", "ҹ�Ӿ�", 44, 100, 100 },
		{ "Thermal Goggles", "�ȳ���ҹ�Ӿ�", 45, 100, 100 },
		{ "Parachute", "����ɡ", 46, 100, 100 }
	} },
	
	{ "�ӵ���", {
		{ "Purple Dildo", "��ɫ�ļ�����", 10, 100, 100 },
		{ "Dildo", "��ɫ���𶯰�(С)", 11, 100, 100 },
		{ "Vibrator", "��ɫ���𶯰�(��)", 12, 100, 100 },
		{ "Silver Vibrator", "��ɫ���𶯰�", 13, 100, 100 },
		{ "Flowers", "����", 14, 100, 100 },
		{ "Cane", "����", 15, 100, 100 }
	} }
};

void showWeaponShopCategoryDialog(Account& player, const std::string& category)
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
		std::stringstream stream;
		stream << item.cnname << " �۸�: " << item.price << " ��ҩ: " << item.ammo;
		list.append(stream.str(), [&player, item]() {
			player.increaseMoney(-1 * item.price, "buy weapon");
			GivePlayerWeapon(player.getInGameID(), item.id, item.ammo);
		}, [&player]() { showWeaponShopDialog(player); });
	}

	GameServer::getInstance().dialogmanager.displayListDialog(player, "��������", list, "����", "����");
}

void showWeaponShopDialog(Account& player)
{
	DialogItemList list;

	for (auto type : g_weaponlist)
	{
		std::string category = type.first;
		list.append(category, [&player, category]() { showWeaponShopCategoryDialog(player, category); });
	}

	GameServer::getInstance().dialogmanager.displayListDialog(player, "��������", list, "ȷ��", "ȡ��");
}
