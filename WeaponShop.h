#pragma once

#include "Account.h"

#include <map>
#include <vector>

struct WeaponShopItem
{
	const char* enname;
	const char* cnname;
	int id;
	int price;
	int ammo;
};

void showWeaponShopCategoryDialog(Account& player, const std::string& category);
void showWeaponShopDialog(Account& player);

