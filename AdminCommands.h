#pragma once

#include "Account.h"

enum AdminCommandType
{
	ADMIN_TARGET_REASON,
	ADMIN_TARGET,
	ADMIN_REASON,
	ADMIN
};

class AdminCommandBase
{
public:
	std::string name;
	int requiredlevel;
	AdminCommandType type;

public:
	AdminCommandBase(const std::string name, int reqlevel, AdminCommandType type, std::vector<AdminCommandBase*>& registration) :
		name(name), requiredlevel(reqlevel), type(type)
	{
		registration.push_back(this);
	}

	virtual void executeA(const std::shared_ptr<Account>& admin) {}
	virtual void executeAT(const std::shared_ptr<Account>& admin, const std::shared_ptr<Account>& target) {}
	virtual void executeAR(const std::shared_ptr<Account>& admin, const std::string& reason) {}
	virtual void executeATR(const std::shared_ptr<Account>& admin, const std::shared_ptr<Account>& target, const std::string& reason) {}
};

void showAdminDialog(const std::shared_ptr<Account>& admin, const std::shared_ptr<Account>& target, std::vector<AdminCommandBase*>& cmdlist);
void showPlayerAdminDialog(const std::shared_ptr<Account>& admin, const std::shared_ptr<Account>& target);
void showServerAdminDialog(const std::shared_ptr<Account>& admin);
void showAdminInputDialog(const std::shared_ptr<Account>& player, const std::function<void(const std::string&)>& callback);
