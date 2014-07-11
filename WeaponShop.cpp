#include <sstream>
#include <string>
#include <sampgdk/a_players.h>

#include "WeaponShop.h"
#include "GameServer.h"

std::map<std::string, std::vector<WeaponShopItem>> g_weaponlist = 
{
	{ "徒手", {
		{ "Fist", "拳头", 0, 100, 100 },
		{ "Brass Knuckles", "铜指虎", 1, 100, 100 },
		{ "Golf Club", "高尔夫球杆", 2, 100, 100 },
		{ "Nightstick", "警棍", 3, 100, 100 },
		{ "Knife", "匕首", 4, 100, 100 },
		{ "Baseball Bat", "棒球棍", 5, 100, 100 },
		{ "Shovel", "铁铲", 6, 100, 100 },
		{ "Pool Cue", "台球杆", 7, 100, 100 },
		{ "Katana", "武士刀", 8, 100, 100 },
		{ "Chainsaw", "电锯", 9, 100, 100 }
	} },

	{ "手枪", {
		{ "9mm", "9mm手枪", 22, 100, 100 },
		{ "Silenced 9mm", "9mm消音手枪", 23, 100, 100 },
		{ "Desert Eagle", "沙漠之鹰手枪", 24, 100, 100 }
	} },

	{ "霰弹枪", {
		{ "Shotgun", "霰弹枪", 25, 100, 100 },
		{ "Sawnoff Shotgun", "短管霰弹枪", 26, 100, 100 },
		{ "Combat Shotgun", "战斗霰弹枪", 27, 100, 100 }
	} },

	{ "冲锋枪", {
		{ "Micro SMG/Uzi", "乌兹冲锋枪 (Micro-UZI)", 28, 100, 100 },
		{ "MP5", "MP5 冲锋枪", 29, 100, 100 },
		{ "Tec-9", "Tec-9 微型冲锋枪", 32, 100, 100 }
	} },

	{ "突击步枪", {
		{ "AK-47", "AK-47 突击步枪", 30, 100, 100 },
		{ "M4", "M4 卡宾枪", 31, 100, 100 }
	} },

	{ "狙击步枪", {
		{ "Country Rifle", "乡村步枪", 33, 100, 100 },
		{ "Sniper Rifle", "狙击步枪", 34, 100, 100 }
	} },

	{ "重型武器", {
		{ "RPG", "火箭发射器 (RPG)", 35, 100, 100 },
		{ "HS Rocket", "热追踪火箭发射器", 36, 100, 100 },
		{ "Flamethrower", "火焰喷射器", 37, 100, 100 },
		{ "Minigun", "迷你炮机枪 (Minigun)", 38, 100, 100 }
	} },
	
	{ "投掷武器", {
		{ "Grenade", "手榴弹", 16, 100, 100 },
		{ "Tear Gas", "催泪弹", 17, 100, 100 },
		{ "Molotov Cocktail", "燃烧弹", 18, 100, 100 },
		{ "Satchel Charge", "炸药包", 39, 100, 100 }
	} },

	{ "装备", {
		{ "Spraycan", "喷漆罐", 41, 100, 100 },
		{ "Fire Extinguisher", "灭火器", 42, 100, 100 },
		{ "Camera", "照相机", 43, 100, 100 },
		{ "Night Vis Goggles", "夜视镜", 44, 100, 100 },
		{ "Thermal Goggles", "热成像夜视镜", 45, 100, 100 },
		{ "Parachute", "降落伞", 46, 100, 100 }
	} },
	
	{ "坑爹货", {
		{ "Purple Dildo", "紫色的假阳具", 10, 100, 100 },
		{ "Dildo", "白色的震动棒(小)", 11, 100, 100 },
		{ "Vibrator", "白色的震动棒(大)", 12, 100, 100 },
		{ "Silver Vibrator", "银色的震动棒", 13, 100, 100 },
		{ "Flowers", "花束", 14, 100, 100 },
		{ "Cane", "拐杖", 15, 100, 100 }
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
		stream << item.cnname << " 价格: " << item.price << " 弹药: " << item.ammo;
		list.append(stream.str(), [&player, item]() {
			player.increaseMoney(-1 * item.price, "buy weapon");
			GivePlayerWeapon(player.getInGameID(), item.id, item.ammo);
		}, [&player]() { showWeaponShopDialog(player); });
	}

	GameServer::getInstance().dialogmanager.displayListDialog(player, "购买武器", list, "购买", "返回");
}

void showWeaponShopDialog(Account& player)
{
	DialogItemList list;

	for (auto type : g_weaponlist)
	{
		std::string category = type.first;
		list.append(category, [&player, category]() { showWeaponShopCategoryDialog(player, category); });
	}

	GameServer::getInstance().dialogmanager.displayListDialog(player, "购买武器", list, "确定", "取消");
}
