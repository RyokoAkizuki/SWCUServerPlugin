#include <sampgdk/a_players.h>
#include <sstream>

#include "AccountDialogs.h"
#include "GameServer.h"

void showLoginDialog(const std::shared_ptr<Account>& player)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "��¼ Login", "����������\nPlease enter your password\n[OK] [Exit]",
		"��¼", "�˳�������", false,
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
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "ע�� Register", "���������6λ������\nPlease use a password longer than 6 chars.\n[OK] [Exit]",
		"ע��", "�˳�������", false,
		[player](const std::string& pw) { player->create(pw); },
		[player](const std::string& pw) { Kick(player->getInGameID()); }
	);
}

void showChangePasswordDialog(const std::shared_ptr<Account>& player)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "��������", "���������6λ������",
		"ȷ��", "ȡ��", false,
		[player](const std::string& pw) { player->changePassword(pw); }
	);
}

void showSetLogNameDialog(const std::shared_ptr<Account>& player)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "���ĵ�¼��", "�������µ�����",
		"ȷ��", "ȡ��", false,
		[player](const std::string& newname) { player->setLogName(newname); }
	);
}

void showSetNicknameDialog(const std::shared_ptr<Account>& player)
{
	GameServer::getInstance().dialogmanager.displayInputDialog(player, "������ʾ��", "�������µ�����",
		"ȷ��", "ȡ��", false,
		[player](const std::string& name) { player->setNickname(name); }
	);
}

void showViewAccountDetailDialog(const std::shared_ptr<Account>& player)
{
	std::stringstream str;
	str << "ID: " << player->getUserID() <<
		"\n��¼��: " << player->getLogName() <<
		"\n��ʾ��: " << player->getNickname() <<
		"\n���п����: " << player->getMoney() <<
		"\n����Ա�ȼ�: " << player->getAdminLevel();
	GameServer::getInstance().dialogmanager.displayMessageDialog(player,
		"�ҵ���Ϣ", str.str(), "ȷ��", "");
}

void showAccountOptionsDialog(const std::shared_ptr<Account>& player)
{
	DialogItemList list;
	list
		.append("�鿴�ҵ���Ϣ", std::bind(&showViewAccountDetailDialog, player))
		.append("��������", std::bind(&showChangePasswordDialog, player))
		.append("���ĵ�¼��", std::bind(&showSetLogNameDialog, player))
		.append("������ʾ��", std::bind(&showSetNicknameDialog, player))
		;

	GameServer::getInstance().dialogmanager.displayListDialog(
		player, "�ҵ��˺�", list, "ȷ��", "ȡ��");
}
