#pragma once

#include "Account.h"

#include <functional>

void showLoginDialog(const std::shared_ptr<Account>& player);
void showRegisterDialog(const std::shared_ptr<Account>& player);
void showChangePasswordDialog(const std::shared_ptr<Account>& player);
void showSetLogNameDialog(const std::shared_ptr<Account>& player);
void showSetNicknameDialog(const std::shared_ptr<Account>& player);
void showViewAccountDetailDialog(const std::shared_ptr<Account>& player);
void showAccountOptionsDialog(const std::shared_ptr<Account>& player);
// cb: void(playerid)
void showSearchPlayerDialog(const std::shared_ptr<Account>& player, const std::function<void(const std::string&)>& cb);
