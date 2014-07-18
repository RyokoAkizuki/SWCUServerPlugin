#pragma once

#include "HouseBuilder.h"
#include "Account.h"

// step one
void showHouseBuildSetOwnerDialog(const std::shared_ptr<Account>& player, const std::shared_ptr<HouseBuilder>& session);
// step two
void showHouseBuildSetNameDialog(const std::shared_ptr<Account>& player, const std::shared_ptr<HouseBuilder>& session);
