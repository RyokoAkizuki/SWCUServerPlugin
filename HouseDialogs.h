#pragma once

#include "Account.h"
#include "House.h"

void showHouseAuthDialog(const std::shared_ptr<Account>& player, const std::shared_ptr<House>& house);
void showHouseListDialog(const std::shared_ptr<Account>& player);
void showHouseManageDialog(const std::shared_ptr<Account>& player, const std::shared_ptr<House>& house);
void showHouseChangeNameDialog(const std::shared_ptr<Account>& player, const std::shared_ptr<House>& house);

// void showHouseGuestDialog(const std::shared_ptr<Account>& player, const std::shared_ptr<House>& house);
