#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <sampgdk/a_players.h>

#include "WeaponShop.h"
#include "GameServer.h"

std::map<std::string, std::vector<WeaponShopItem>> g_weaponlist = 
{
	{ "徒手", {
		//{ "Fist", "拳头", 0, 0, 1000 },
		{ "Brass Knuckles", "铜指虎", 1, 0, 1000 },
		{ "Golf Club", "高尔夫球杆", 2, 0, 1000 },
		{ "Nightstick", "警棍", 3, 0, 1000 },
		{ "Knife", "匕首", 4, 0, 1000 },
		{ "Baseball Bat", "棒球棍", 5, 0, 1000 },
		{ "Shovel", "铁铲", 6, 0, 1000 },
		{ "Pool Cue", "台球杆", 7, 0, 1000 },
		{ "Katana", "武士刀", 8, 0, 1000 },
		{ "Chainsaw", "电锯", 9, 0, 1000 }
	} },

	{ "手枪", {
		{ "9mm", "9mm手枪", 22, 0, 1000 },
		{ "Silenced 9mm", "9mm消音手枪", 23, 0, 1000 },
		{ "Desert Eagle", "沙漠之鹰手枪", 24, 0, 1000 }
	} },

	{ "霰弹枪", {
		{ "Shotgun", "霰弹枪", 25, 0, 1000 },
		{ "Sawnoff Shotgun", "短管霰弹枪", 26, 0, 1000 },
		{ "Combat Shotgun", "战斗霰弹枪", 27, 0, 1000 }
	} },

	{ "冲锋枪", {
		{ "Micro SMG/Uzi", "乌兹冲锋枪 (Micro-UZI)", 28, 0, 1000 },
		{ "MP5", "MP5 冲锋枪", 29, 0, 1000 },
		{ "Tec-9", "Tec-9 微型冲锋枪", 32, 0, 1000 }
	} },

	{ "突击步枪", {
		{ "AK-47", "AK-47 突击步枪", 30, 0, 1000 },
		{ "M4", "M4 卡宾枪", 31, 0, 1000 }
	} },

	{ "狙击步枪", {
		{ "Country Rifle", "乡村步枪", 33, 0, 1000 },
		{ "Sniper Rifle", "狙击步枪", 34, 0, 1000 }
	} },

	{ "重型武器", {
		{ "RPG", "火箭发射器 (RPG)", 35, 0, 1000 },
		{ "HS Rocket", "热追踪火箭发射器", 36, 0, 1000 },
		{ "Flamethrower", "火焰喷射器", 37, 0, 1000 },
		//{ "Minigun", "迷你炮机枪 (Minigun)", 38, 0, 1000 }
	} },
	
	{ "投掷武器", {
		{ "Grenade", "手榴弹", 16, 0, 1000 },
		{ "Tear Gas", "催泪弹", 17, 0, 1000 },
		{ "Molotov Cocktail", "燃烧弹", 18, 0, 1000 },
		{ "Satchel Charge", "炸药包", 39, 0, 1000 }
	} },

	{ "装备", {
		{ "Spraycan", "喷漆罐", 41, 0, 1000 },
		{ "Fire Extinguisher", "灭火器", 42, 0, 1000 },
		{ "Camera", "照相机", 43, 0, 1000 },
		//{ "Night Vis Goggles", "夜视镜", 44, 0, 1000 },
		//{ "Thermal Goggles", "热成像夜视镜", 45, 0, 1000 },
		{ "Parachute", "降落伞", 46, 0, 1000 }
	} },
	
	{ "其他物品", {
		{ "Purple Dildo", "紫色的假阳具", 10, 0, 1000 },
		{ "Dildo", "白色的震动棒(小)", 11, 0, 1000 },
		{ "Vibrator", "白色的震动棒(大)", 12, 0, 1000 },
		{ "Silver Vibrator", "银色的震动棒", 13, 0, 1000 },
		{ "Flowers", "花束", 14, 0, 1000 },
		{ "Cane", "拐杖", 15, 0, 1000 }
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
		std::stringstream stream;
		stream << item.cnname << " 价格: " << item.price << " 弹药: " << item.ammo;
		list.append(stream.str(), [player, item]() {
			player->increaseMoney(-1 * item.price, "buy weapon");
			GivePlayerWeapon(player->getInGameID(), item.id, item.ammo);
		}, std::bind(&showWeaponShopDialog, player));
	}

	GameServer::getInstance().dialogmanager.displayListDialog(player, "购买武器", list, "购买", "返回");
}

void showWeaponShopDialog(const std::shared_ptr<Account>& player)
{
	DialogItemList list;

	for (auto type : g_weaponlist)
	{
		std::string category = type.first;
		list.append(category, std::bind(&showWeaponShopCategoryDialog, player, category));
	}

	list.append("护甲", [player]() {
		SetPlayerArmour(player->getInGameID(), 100.0f);
	});

	GameServer::getInstance().dialogmanager.displayListDialog(player, "购买武器", list, "确定", "取消");
}
