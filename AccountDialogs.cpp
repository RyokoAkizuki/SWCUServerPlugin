#include <sampgdk/a_players.h>
#include <sstream>

#include "AccountDialogs.h"
#include "GameServer.h"

void showLoginDialog(const std::shared_ptr<Account>& player)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "登录 Login", "请输入密码\nPlease enter your password\n[OK] [Exit]",
		"登录", "退出服务器", false,
		[player](const std::string& pw) {
			if (!player->auth(pw))
			{
				showLoginDialog(player);
			}
		},
		[player](const std::string& pw) { Kick(player->getInGameID()); }
	);
}

void showRegisterDialog(const std::shared_ptr<Account>& player)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "注册 Register", "请输入大于6位的密码\nPlease use a password longer than 6 chars.\n[OK] [Exit]",
		"注册", "退出服务器", false,
		[player](const std::string& pw) { player->create(pw); },
		[player](const std::string& pw) { Kick(player->getInGameID()); }
	);
}

void showChangePasswordDialog(const std::shared_ptr<Account>& player)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "更改密码", "请输入大于6位的密码",
		"确定", "取消", false,
		[player](const std::string& pw) { player->changePassword(pw); }
	);
}

void showSetLogNameDialog(const std::shared_ptr<Account>& player)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "更改登录名", "请输入新的名称",
		"确定", "取消", false,
		[player](const std::string& newname) { player->setLogName(newname); }
	);
}

void showSetNicknameDialog(const std::shared_ptr<Account>& player)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "更改显示名", "请输入新的名称",
		"确定", "取消", false,
		[player](const std::string& name) { player->setNickname(name); }
	);
}

void showViewAccountDetailDialog(const std::shared_ptr<Account>& player)
{
	std::stringstream str;
	str << "ID: " << player->getUserID() <<
		"\n登录名: " << player->getLogName() <<
		"\n显示名: " << player->getNickname() <<
		"\n银行卡余额: " << player->getMoney() <<
		"\n管理员等级: " << player->getAdminLevel();
	GameServer::getInstance().dialogmanager.displayMessageDialog(player,
		"我的信息", str.str(), "确定", "");
}

void showAccountOptionsDialog(const std::shared_ptr<Account>& player)
{
	DialogItemList list;
	list
		.append("查看我的信息", std::bind(&showViewAccountDetailDialog, player))
		.append("更改密码", std::bind(&showChangePasswordDialog, player))
		.append("更改登录名", std::bind(&showSetLogNameDialog, player))
		.append("更改显示名", std::bind(&showSetNicknameDialog, player))
		;

	GameServer::getInstance().dialogmanager.displayListDialog(
		player, "我的账号", list, "确定", "取消");
}
