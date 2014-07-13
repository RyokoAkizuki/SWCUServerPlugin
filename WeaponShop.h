#pragma once

#include "Account.h"

struct WeaponShopItem
{
	const char* enname;
	const char* cnname;
	int id;
	int price;
	int ammo;
};

void showWeaponShopCategoryDialog(const std::shared_ptr<Account>& player, const std::string& category);
void showWeaponShopDialog(const std::shared_ptr<Account>& player);
